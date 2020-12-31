#pragma once

#include <any>
#include <string>
#include <vector>
#include "permissible.hpp"

namespace chattere
{
    class Server;
    class Permissible;
    class User;
    class CommandSender
    {
    public:
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
        virtual void SendMessage(const std::vector<std::string> &messages) const;
        // virtual void SendMessage(const std::int64_t sender, const std::string &message) const;
        // virtual void SendMessage(const std::int64_t sender, const std::vector<std::string> &messages) const;

        virtual bool IsUser() const;
        virtual bool IsConsole() const;
    };
} // namespace chattere