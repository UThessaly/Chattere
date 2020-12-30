#pragma once

#include "server_socket.hpp"
#include "packet.hpp"
#include "database.hpp"
#include "actions.hpp"
#include "packet.hpp"
#include <spdlog/spdlog.h>

namespace chattere
{
    class Server
    {
    public:
        Server(std::uint16_t port = 25565);

        void Listen();
        std::shared_ptr<spdlog::logger> GetConsoleLogger();

        database::Database &GetDatabase();

        void AssingSocketToUser(std::int64_t socket, std::int64_t user_Id);
        std::int64_t UserFromClient(std::int64_t client_id);

    private:
        void ProcessPacket(std::shared_ptr<net::ClientSocket> client, protocol::Packet &packet);

        chattere::net::ServerSocket m_server;
        Handlers<protocol::Packet::DataCase, std::tuple<Server *, std::shared_ptr<net::ClientSocket>, protocol::Packet *>> m_packet_handlers;
        friend decltype(m_packet_handlers);

        database::Database m_database;
        std::shared_ptr<spdlog::logger> m_console_logger;
        std::map<std::int64_t, std::int64_t> m_socket_to_user;
    };
} // namespace chattere