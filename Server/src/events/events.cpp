#include "events.hpp"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <algorithm>
#include <numeric>

namespace chattere
{

    BasicEvent::BasicEvent(Server *server, const std::string &name)
    {
    }

    const std::string &BasicEvent::GetName()
    {
        return m_name;
    }

    Server *BasicEvent::GetServer()
    {
        return m_server;
    }

    bool BasicEvent::IsCanceled()
    {
        return m_is_canceled;
    }

    void BasicEvent::SetCanceled(bool cancel)
    {
        m_is_canceled = true;
    }

    UserBasicEvent::UserBasicEvent(Server *server, const std::string &event_name, std::shared_ptr<User> user, std::shared_ptr<net::ClientSocket> client)
        : BasicEvent(server, event_name),
          m_client(client),
          m_user(user)
    {
    }

    std::shared_ptr<User> UserBasicEvent::GetUser()
    {
        return m_user;
    }

    std::shared_ptr<net::ClientSocket> UserBasicEvent::GetClient()
    {
        return m_client;
    }

    UserChatEvent::UserChatEvent(Server *server, std::shared_ptr<User> user, std::shared_ptr<net::ClientSocket> client, std::string &message)
        : UserBasicEvent(server, "OnUserChatEvent", user, client),
          m_message(message),
          m_format("<$0> $1")
    {
    }

    void UserChatEvent::SetMessage(std::string &message)
    {
        m_message = message;
    }

    void UserChatEvent::SetFormat(std::string &format)
    {
        m_format = format;
    }

    std::string &UserChatEvent::GetMessage()
    {
        return m_message;
    }

    std::string &UserChatEvent::GetFormat()
    {
        return m_format;
    }

    void EventListener::OnUserChatEvent(std::shared_ptr<UserChatEvent> event)
    {
    }

    OnUserCommand::OnUserCommand(Server *server, std::shared_ptr<User> user, std::shared_ptr<net::ClientSocket> client, std::string &command_message)
        : UserBasicEvent(server, "OnUserCommand", user, client)
    {
        SetCommandMessage(command_message);
    }

    void OnUserCommand::SetCommandMessage(const std::string &message)
    {
        std::vector<std::string> args;
        boost::split(args, message, boost::is_any_of(" "), boost::token_compress_on);

        m_command = args[0];
        m_args = args;
    }

    void OnUserCommand::SetCommand(const std::string &command)
    {
        m_command = command;
        m_args[0] = command;
    }

    void OnUserCommand::SetCommandArgs(const std::vector<std::string> &args)
    {
        m_args = args;
        m_command = args[0];
    }

    const std::string &OnUserCommand::GetCommand()
    {
        return m_command;
    }

    const std::vector<std::string> &OnUserCommand::GetArgsArray()
    {
        return m_args;
    }

    const std::string OnUserCommand::GetFullCommand()
    {
        std::string result;

        for (const auto &arg : result)
        {
            result += arg + ' ';
        }

        result.resize(result.size() - 1);
        return std::move(result);
    }

} // namespace chattere