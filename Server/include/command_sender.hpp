#pragma once

#include <any>
#include <string>
#include <vector>
#include "permissible.hpp"
#include "client_socket.hpp"

namespace chattere
{
    class Server;
    class Permissible;
    class User;

    /**
     * @brief The CommandSender base class 
     */
    class CommandSender
    {
    public:
        // CommandSender(Server *server, std::shared_ptr<net::ClientSocket> client, std::shared_ptr<User> user);
        /**
         * @brief Get the Name of thje Command Sender
         * 
         * @return const std::string& 
         */
        virtual const std::string GetName() const;

        /**
         * @brief Get the Server object
         * 
         * @return Server* 
         */
        virtual Server *GetServer() const;

        /**
         * @brief Sends a message to the command sender
         * 
         * @param message 
         */
        virtual void SendMessage(const std::string &message) const;

        /**
         * @brief Sends a list of messages to the command sender
         * 
         * @param messages 
         */
        // virtual void SendMessage(const std::vector<std::string> &messages) const;
        // virtual void SendMessage(const std::int64_t sender, const std::string &message) const;
        // virtual void SendMessage(const std::int64_t sender, const std::vector<std::string> &messages) const;

        // std::shared_ptr<User> GetUser() const;
        // std::shared_ptr<net::ClientSocket> GetClient() const;

        virtual bool IsUser() const;
        virtual bool IsConsole() const;

    private:
        std::shared_ptr<net::ClientSocket> m_client;
        std::shared_ptr<User> m_user;
        Server *server;
    };
} // namespace chattere