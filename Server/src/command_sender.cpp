#include "command_sender.hpp"
#include <spdlog/spdlog.h>

namespace chattere
{
    //  CommandSender::CommandSender(Server *server, std::shared_ptr<net::ClientSocket> client, std::shared_ptr<User> user) { 

    //  }
       

    // void CommandSender::SendMessage(const std::vector<std::string> &messages) const
    // {
    //     for (const auto &message : messages)
    //         SendMessage(message);
    // }

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
        return "hi";
    }

    bool CommandSender::IsUser() const
    {
        return false;
    }

    bool CommandSender::IsConsole() const
    {
        return false;
    }

    // std::shared_ptr<User> CommandSender::GetUser() const {
    //     return m_user;
    // }

    // std::shared_ptr<net::ClientSocket> CommandSender::GetClient() const {
    //     return m_client;
    // }

} // namespace chattere