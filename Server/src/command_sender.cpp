#include "command_sender.hpp"
#include <spdlog/spdlog.h>

namespace chattere
{
    // CommandSender::CommandSender() {}

    void CommandSender::SendMessage(const std::vector<std::string> &messages) const
    {
        for (const auto &message : messages)
            SendMessage(message);
    }

    void CommandSender::SendMessage(const std::string &message) const
    {
        spdlog::critical("ComamndSender::SendMessage(message) => Command sender not found");
    }

    Server *CommandSender::GetServer() const
    {
        return nullptr;
    }

    const std::string CommandSender::GetName() const
    {
        return "Unknwon Command Sender";
    }

    bool CommandSender::IsUser() const
    {
        return false;
    }

    bool CommandSender::IsConsole() const
    {
        return false;
    }

} // namespace chattere