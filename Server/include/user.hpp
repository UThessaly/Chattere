#pragma once
#include "command_sender.hpp"
#include "packet.hpp"
#include "channel.hpp"
#include "client_socket.hpp"

namespace chattere
{
    class Server;

    class User : public CommandSender
    {
    public:
        User(Server *server, std::shared_ptr<protocol::User> data);

        std::int64_t GetId();
        const std::string GetName() const;
        Server *GetServer() const;
        bool IsUser() const;
        bool IsConsole() const;
        void SendMessage(const std::string &message);

        void AddClientSocket(std::shared_ptr<net::ClientSocket> client);

    private:
        Server *m_server;
        std::shared_ptr<protocol::User> m_user;

        std::vector<std::shared_ptr<net::ClientSocket>> m_sockets;
    };

} // namespace chattere