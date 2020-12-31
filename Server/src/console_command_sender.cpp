#include "console_command_sender.hpp"
#include "server.hpp"

namespace chattere
{
    ConsoleCommandSender::ConsoleCommandSender(Server *server) : m_server(server) {}

    const std::string ConsoleCommandSender::GetName()
    {
        return "Console";
    }

    Server *ConsoleCommandSender::GetServer()
    {
        return m_server;
    }

    void ConsoleCommandSender::SendMessage(const std::string &message) const
    {
        // m_server->GetConsoleLogger()->info("ConsoleCommandSender: {}", message);
    }

    bool ConsoleCommandSender::IsConsole()
    {
        return true;
    }

} // namespace chattere