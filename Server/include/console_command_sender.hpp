#pragma once

#include "command_sender.hpp"

namespace chattere
{
    class ConsoleCommandSender : public CommandSender
    {
    public:
        ConsoleCommandSender(Server *server);

    public:
        const std::string GetName();
        Server *GetServer();
        void SendMessage(const std::string &message) const;

        bool IsConsole();

    private:
        Server *m_server;
    };
} // namespace chattere