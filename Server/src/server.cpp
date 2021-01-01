#include <spdlog/spdlog.h>
#include "server.hpp"
#include <future>
#include "spdlog/sinks/stdout_color_sinks.h"
#include <stdlib.h>
#include "handlers.hpp"
#include "emoji.hpp"

#include "user.hpp"
#include "command_sender.hpp"
#include "permissible.hpp"
#include "channel.hpp"
#include "console_command_sender.hpp"

#include "events.hpp"
#include "server_events.hpp"

namespace chattere
{
    using chattere::net::ServerSocket;
    Server::Server(std::uint16_t port) : m_server(ServerSocket(port)),
                                         m_database(database::Database("data.db3")),
                                         m_event_handler(std::make_shared<ServerEventHandler>(this))
    {
        m_port = port;
        m_console_logger = spdlog::stdout_color_st("Server/Main");
        m_console_logger->set_pattern("[%H:%M:%S %^%L%$] %v");

        m_packet_handlers.Add(chattere::protocol::Packet::DataCase::kAuthRequest, chattere::handlers::serverbound::OnAuthRequest);
        m_packet_handlers.Add(chattere::protocol::Packet::DataCase::kSignupRequest, chattere::handlers::serverbound::OnSignupRequest);
        m_packet_handlers.Add(chattere::protocol::Packet::DataCase::kChat, chattere::handlers::serverbound::OnChat);

        std::shared_ptr basic_listeners = std::make_shared<BasicServerEventListener>();
        auto listener_ptr = std::dynamic_pointer_cast<EventListener>(basic_listeners);
        m_event_handler->RegisterEventListener(listener_ptr);
    }

    void Server::AssingSocketToUser(std::int64_t client_id, std::shared_ptr<User> user)
    {
        m_socket_to_user.insert_or_assign(client_id, user);
    }

    std::shared_ptr<User> Server::UserFromClient(std::int64_t client_id)
    {
        if (m_socket_to_user.count(client_id) == 0)
            return nullptr;
        return m_socket_to_user.at(client_id);
    }

    std::shared_ptr<spdlog::logger> Server::GetConsoleLogger()
    {
        return m_console_logger;
    }

    void Server::Listen()
    {
        m_server.Bind();
        m_server.Queue(5);

        switch (auto threads_to_create = std::any_cast<int>(m_settings["threads"]))
        {
        case 1:
            SingleThreaded();
        case 2:
            Create2Threads();
        default:
            Multithreaded();
            break;
        }
    }

    void Server::Create2Threads()
    {
        m_futures.push_back(std::async(std::launch::async, [&]() {
            while (true)
            {
                auto client = m_server.Accept();
                if (client != nullptr)
                {
                    GetConsoleLogger()->info("\t{}\tClient with ID {} has connected", chattere::EMOJIS["link"], client->GetId());
                }

                m_server.ClientsRecv();
            }
        }));

        m_futures.push_back(std::async(std::launch::async, [&]() {
            std::vector<std::uint8_t> packet_buffer;
            while (true)
            {
                auto &&clients = m_server.GetClients();
                for (auto &[id, client] : clients)
                {
                    {
                        std::lock_guard<std::mutex> guard(client->GetMutex());
                        packet_buffer = client->m_read_buffer;
                    }
                    auto [len, packets] = protocol::packets_from_buffer(&packet_buffer);

                    if (len <= 0)
                        continue;
                    spdlog::debug("Reading for client {}, {} bytes", client->GetId(), len);

                    {
                        std::lock_guard<std::mutex> guard(client->GetMutex());
                        auto &read_buffer = client->m_read_buffer;
                        read_buffer.erase(read_buffer.begin(), read_buffer.size() > len ? read_buffer.begin() + len : read_buffer.end());
                    }

                    {
                        std::lock_guard<std::mutex> guard(m_unprocessed_packet_mutex);
                        for (auto &packet : packets)

                        {
                            std::shared_ptr packet_ptr = std::make_shared<protocol::Packet>(packet);
                            ProcessPacket(client, packet_ptr);
                        }
                    }
                }
            }
        }));
    }

    void Server::SingleThreaded()
    {
        while (true)
        {
            auto client = m_server.Accept();
            if (client != nullptr)
            {
                GetConsoleLogger()->info("\t{}\tClient with ID {} has connected", chattere::EMOJIS["link"], client->GetId());
            }

            m_server.ClientsRecv();

            auto &&clients = m_server.GetClients();
            for (auto &[id, client] : clients)
            {
                auto &packet_buffer = client->m_read_buffer;
                auto [len, packets] = protocol::packets_from_buffer(&packet_buffer);

                if (len <= 0)
                    continue;
                spdlog::debug("Reading for client {}, {} bytes", client->GetId(), len);

                auto &read_buffer = client->m_read_buffer;
                read_buffer.erase(read_buffer.begin(), read_buffer.size() > len ? read_buffer.begin() + len : read_buffer.end());

                for (auto &packet : packets)
                {
                    std::shared_ptr packet_ptr = std::make_shared<protocol::Packet>(packet);
                    ProcessPacket(client, packet_ptr);
                }
            }
        }
    }

    void Server::Multithreaded()
    {
        m_futures.push_back(std::async(std::launch::async, [&]() {
            while (true)
            {
                auto client = m_server.Accept();
                if (client != nullptr)
                {
                    GetConsoleLogger()->info("\t{}\tClient with ID {} has connected", chattere::EMOJIS["link"], client->GetId());
                }

                m_server.ClientsRecv();
            }
        }));

        // Merged this block of code with the thread above, so Accept() and Recv() are on the same thread
        // m_futures.push_back(std::async(std::launch::async, [&]() {
        //     while (true)
        //     {
        //         m_server.ClientsRecv();
        //         spdlog::info("test");
        //     }
        // }));

        m_futures.push_back(std::async(std::launch::async, [&]() {
            std::vector<std::uint8_t> packet_buffer;
            while (true)
            {
                auto &&clients = m_server.GetClients();
                for (auto &[id, client] : clients)
                {
                    {
                        std::lock_guard<std::mutex> guard(client->GetMutex());
                        packet_buffer = client->m_read_buffer;
                    }
                    auto [len, packets] = protocol::packets_from_buffer(&packet_buffer);

                    if (len <= 0)
                        continue;
                    spdlog::debug("Reading for client {}, {} bytes", client->GetId(), len);

                    {
                        std::lock_guard<std::mutex> guard(client->GetMutex());
                        auto &read_buffer = client->m_read_buffer;
                        read_buffer.erase(read_buffer.begin(), read_buffer.size() > len ? read_buffer.begin() + len : read_buffer.end());
                    }

                    {
                        std::lock_guard<std::mutex> guard(m_unprocessed_packet_mutex);
                        for (auto &packet : packets)

                        {
                            std::shared_ptr packet_ptr = std::make_shared<protocol::Packet>(packet);
                            m_unprocessed_packets.push_back(std::make_tuple(client, packet_ptr));
                        }
                    }
                }
            }
        }));

        for (int i = 0; i < std::any_cast<int>(m_settings["threads"]) - 2; i++)
        {
            const auto id = CreateSnowflake();
            m_futures.push_back(std::async(std::launch::async, [&]() {
                while (true)
                {
                    std::lock_guard<std::mutex> guard(m_unprocessed_packet_mutex);
                    if (m_unprocessed_packets.size() > 0)
                    {
                        auto [client, packet] = m_unprocessed_packets[0];
                        m_unprocessed_packets.pop_front();

                        ProcessPacket(client, packet);
                        GetConsoleLogger()->debug("Processing Packet on thread {}", id);
                    }
                }
            }));
        }
    }

    void Server::ProcessPacket(std::shared_ptr<net::ClientSocket> client, std::shared_ptr<protocol::Packet> packet)
    {
        auto data = std::make_tuple(this, client, packet);
        GetConsoleLogger()->debug("\t{}\tProcessing Packet of Type {} from client {}", chattere::EMOJIS["card_file_box"], packet->data_case(), client->GetId());
        m_packet_handlers.Run(packet->data_case(), data);
        // spdlog::info("Processing Packet from {}", client->GetId());
        // auto request = packet.auth_request();
        // spdlog::info("Client {} is requesting to login with {} {}", client->GetId(), request.username(), request.password());
    }

    std::uint16_t Server::GetPort() const
    {
        return m_port;
    }

    database::Database &Server::GetDatabase()
    {
        return m_database;
    }

    std::mutex &Server::GetGeneralMutex()
    {
        return m_general_mutex;
    }

    void Server::SetProperty(std::string property, std::any value)
    {
        m_settings[property] = value;
    }

    std::shared_ptr<ServerEventHandler> Server::GetEventHandlers()
    {
        return m_event_handler;
    }
} // namespace chattere