#include <spdlog/spdlog.h>
#include "server.hpp"
#include <future>
#include "spdlog/sinks/stdout_color_sinks.h"
#include <stdlib.h>
#include "handlers.hpp"
#include "emoji.hpp"

namespace chattere
{
    using chattere::net::ServerSocket;
    Server::Server(std::uint16_t port) : m_server(ServerSocket(port)),
                                         m_database(database::Database("data.db3"))
    {
        m_console_logger = spdlog::stdout_color_st("Server/Main");
        m_console_logger->set_pattern("[%H:%M:%S %^%L%$] %v");

        m_packet_handlers.Add(chattere::protocol::Packet::DataCase::kAuthRequest, chattere::handlers::serverbound::OnAuthRequest);
        m_packet_handlers.Add(chattere::protocol::Packet::DataCase::kSignupRequest, chattere::handlers::serverbound::OnSignupRequest);
        m_packet_handlers.Add(chattere::protocol::Packet::DataCase::kChat, chattere::handlers::serverbound::OnChat);
    }

    void Server::AssingSocketToUser(std::int64_t client_id, std::int64_t user_id)
    {
        m_socket_to_user.insert_or_assign(client_id, user_id);
    }

    std::int64_t Server::UserFromClient(std::int64_t client_id)
    {
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

        std::vector<std::future<void>> futures;
        futures.push_back(std::async(std::launch::async, [&]() {
            while (true)
            {
                auto client = m_server.Accept();
                GetConsoleLogger()->info("\t{}\tClient with ID {} has connected", chattere::EMOJIS["link"], client->GetId());
            }
        }));

        futures.push_back(std::async(std::launch::async, [&]() {
            while (true)
            {
                m_server.ClientsRecv();

                auto clients = m_server.GetClients();
                for (auto &[id, client] : clients)
                {
                    auto [len, packets] = protocol::packets_from_buffer(&client->m_read_buffer);

                    if (len <= 0)
                        continue;
                    spdlog::debug("Reading for client {}, {} bytes", client->GetId(), len);
                    auto &buf = client->m_read_buffer;

                    client->m_read_buffer.erase(buf.begin(), buf.size() > len ? buf.begin() + len : buf.end());

                    for (auto &packet : packets)
                    {
                        ProcessPacket(client, packet);
                    }
                }
            }
        }));
    }

    void Server::ProcessPacket(std::shared_ptr<net::ClientSocket> client, protocol::Packet &packet)
    {
        auto data = std::make_tuple(this, client, &packet);
        GetConsoleLogger()->debug("\t{}\tProcessing Packet of Type {} from client {}", chattere::EMOJIS["card_file_box"], packet.data_case(), client->GetId());
        m_packet_handlers.Run(packet.data_case(), data);
        // spdlog::info("Processing Packet from {}", client->GetId());
        // auto request = packet.auth_request();
        // spdlog::info("Client {} is requesting to login with {} {}", client->GetId(), request.username(), request.password());
    }

    database::Database &Server::GetDatabase()
    {
        return m_database;
    }
} // namespace chattere