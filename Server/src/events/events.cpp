#include "events.hpp"

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

    UserChatEvent::UserChatEvent(Server *server, std::shared_ptr<User> user, std::shared_ptr<net::ClientSocket> client, std::string &message)
        : BasicEvent(server, "OnUserChatEvent"),
          m_user(user),
          m_client(client),
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

    std::shared_ptr<User> UserChatEvent::GetUser()
    {
        return m_user;
    }

    std::shared_ptr<net::ClientSocket> UserChatEvent::GetClient()
    {
        return m_client;
    }

    void EventListener::OnUserChatEvent(std::shared_ptr<UserChatEvent> event)
    {
    }

} // namespace chattere