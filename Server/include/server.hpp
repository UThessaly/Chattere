#pragma once

#include "server_socket.hpp"
#include "packet.hpp"
#include "database.hpp"
#include "actions.hpp"
#include "packet.hpp"
#include <spdlog/spdlog.h>
#include <mutex>
#include <any>
#include <deque>
#include <future>
#include "command_sender.hpp"
#include "user.hpp"
#include "events.hpp"
#include "server_events.hpp"
#include <yaml-cpp/yaml.h>

namespace chattere
{
    class User;
    class Server
    {
    public:
        Server(std::uint16_t port = 25565);

        void Listen();
        std::shared_ptr<spdlog::logger> GetConsoleLogger();

        database::Database &GetDatabase();

        void AssingSocketToUser(std::int64_t socket, std::shared_ptr<User> user);
        std::shared_ptr<User> UserFromClient(std::int64_t client_id);

        std::uint16_t GetPort() const;
        std::mutex &GetGeneralMutex();

        void SetProperty(std::string property, std::any value);

        std::vector<std::future<void>> m_futures;

        std::shared_ptr<ServerEventHandler> GetEventHandlers();

        const std::string ColorFormat(const std::string &message) const;

        YAML::Node &GetConfig();

    private:
        YAML::Node CreateDefaultConfig() const;

        YAML::Node m_config;
        std::mutex m_general_mutex;
        std::mutex m_unprocessed_packet_mutex;
        std::deque<std::tuple<std::shared_ptr<net::ClientSocket>, std::shared_ptr<protocol::Packet>>> m_unprocessed_packets;
        void ProcessPacket(std::shared_ptr<net::ClientSocket> client, std::shared_ptr<protocol::Packet> packet);
        std::map<std::string, std::any> m_settings = {{"threads", 1}};

        void Multithreaded();
        void Create2Threads();
        void SingleThreaded();

        chattere::net::ServerSocket m_server;
        Handlers<protocol::Packet::DataCase, std::tuple<Server *, std::shared_ptr<net::ClientSocket>, std::shared_ptr<protocol::Packet>>> m_packet_handlers;
        friend decltype(m_packet_handlers);

        database::Database m_database;
        std::uint16_t m_port;
        std::shared_ptr<spdlog::logger> m_console_logger;

        std::map<std::int64_t, std::shared_ptr<User>> m_socket_to_user;

        std::shared_ptr<ServerEventHandler> m_event_handler;
    };
} // namespace chattere