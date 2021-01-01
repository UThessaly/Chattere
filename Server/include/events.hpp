#pragma once

#include "actions.hpp"
#include "client_socket.hpp"

namespace chattere
{
    class User;
    class Server;

    class BasicEvent
    {
    public:
        BasicEvent(Server *server, const std::string &name);
        const std::string &GetName();
        Server *GetServer();
        bool IsCanceled();
        void SetCanceled(bool cancel);

    private:
        Server *m_server;
        bool m_is_canceled = false;
        const std::string m_name;
    };

    class UserChatEvent : public BasicEvent
    {
    public:
        UserChatEvent(Server *server, std::shared_ptr<User> user, std::shared_ptr<net::ClientSocket> client, std::string &message);

        void SetMessage(std::string &message);
        std::string &GetMessage();
        std::string &GetFormat();
        void SetFormat(std::string &format);
        std::shared_ptr<User> GetUser();
        std::shared_ptr<net::ClientSocket> GetClient();

    private:
        std::string m_message;
        std::string m_format;
        std::shared_ptr<User> m_user;
        std::shared_ptr<net::ClientSocket> m_client;
    };

    class OnUserCommand : public BasicEvent
    {
    };

    class EventListener
    {
    public:
        virtual void OnUserChatEvent(std::shared_ptr<UserChatEvent> event);
    };

} // namespace chattere