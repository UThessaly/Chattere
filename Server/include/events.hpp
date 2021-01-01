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

    class UserBasicEvent : public BasicEvent
    {
    public:
        UserBasicEvent(Server *server, const std::string &event_name, std::shared_ptr<User> user, std::shared_ptr<net::ClientSocket> client);

        std::shared_ptr<User> GetUser();
        std::shared_ptr<net::ClientSocket> GetClient();

    private:
        std::shared_ptr<User> m_user;
        std::shared_ptr<net::ClientSocket> m_client;
    };

    class UserChatEvent : public UserBasicEvent
    {
    public:
        UserChatEvent(Server *server, std::shared_ptr<User> user, std::shared_ptr<net::ClientSocket> client, std::string &message);

        void SetMessage(std::string &message);
        std::string &GetMessage();
        std::string &GetFormat();
        void SetFormat(std::string &format);

    private:
        std::string m_message;
        std::string m_format;
    };

    class OnUserCommand : public BasicEvent
    {
    public:
        OnUserCommand(Server *server, std::shared_ptr<User> user, std::shared_ptr<net::ClientSocket> client, std::string &command_message);

        void SetCommandMessage(std::string &command_message);
        void SetCommandArgs(std::string &args);
        void SetCommandArgs(std::vector<std::string> &args);

        void SetCommand(std::string &command);
    };

    class EventListener
    {
    public:
        virtual void OnUserChatEvent(std::shared_ptr<UserChatEvent> event);
    };

} // namespace chattere