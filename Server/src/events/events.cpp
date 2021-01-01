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

} // namespace chattere