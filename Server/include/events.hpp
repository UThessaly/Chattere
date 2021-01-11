#pragma once

#include "actions.hpp"
#include "client_socket.hpp"
#include "command.hpp"
#include "command_sender.hpp"

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

        void SetMessage(const std::string &message);
        std::string &GetMessage();
        std::string &GetFormat();
        void SetFormat(const std::string &format);

    private:
        std::string m_message;
        std::string m_format;
    };

    class CommandEvent : public BasicEvent
    {
    public:
        CommandEvent(Server *server, std::shared_ptr<CommandSender> sender, std::string &command_message);

        void SetCommandMessage(const std::string &command_message);
        // void SetCommandArgs(std::string &args);
        void SetCommandArgs(const std::vector<std::string> &args);
        void SetCommand(const std::string &command);
        void SetCommandExecutor(const CommandExecutor &executor);

        std::shared_ptr<CommandSender> GetCommandSender();
        const std::string &GetCommand();
        const std::vector<std::string> &GetArgsArray();
        const std::string GetFullCommand();
        const CommandExecutor &GetCommandExecutor();

    private:
        std::string m_command;
        std::vector<std::string> m_args;
        CommandExecutor m_executor;
        std::shared_ptr<CommandSender> m_sender;
    };

    class EventListener
    {
    public:
        virtual void OnUserChatEvent(std::shared_ptr<UserChatEvent> event);
        virtual void OnCommandEvent(std::shared_ptr<CommandEvent> event);
    };

} // namespace chattere