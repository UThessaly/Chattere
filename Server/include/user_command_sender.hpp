#pragma once

#include "command_sender.hpp"
#include "client_socket.hpp"
#include "user.hpp"

namespace chattere
{
    // class UserCommandSender : public CommandSender
    // {
    // public:
    //     UserCommandSender(Server *server, std::shared_ptr<net::ClientSocket> client, std::shared_ptr<User> user);

    // public:
    //     const std::string GetName();
    //     Server *GetServer();
    //     void SendMessage(const std::string &message) const;

    //     bool IsUser();
    //     bool IsConsole();

    // public:
    //     std::shared_ptr<net::ClientSocket> GetClient();
    //     std::shared_ptr<User> GetUser();

    // private:
    //     Server *m_server;
    //     std::shared_ptr<net::ClientSocket> m_client;
    //     std::shared_ptr<User> m_user;
    // };
} // namespace chattere