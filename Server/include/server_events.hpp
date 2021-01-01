#pragma once

#include "events.hpp"

namespace chattere
{
    class EventListener;
    class ServerEventHandler
    {
    public:
        ServerEventHandler(Server *server);

        void RegisterEventListener(std::shared_ptr<EventListener> listener);
        void EmitUserChatEvent(std::shared_ptr<net::ClientSocket> client, std::shared_ptr<User> user, std::string &message);

    private:
        friend Server;

        Server *m_server;

        std::vector<std::shared_ptr<EventListener>> m_listeners{};
    };

    class BasicServerEventListener : public EventListener
    {
        // BasicServerEventListener(Server *server);
        void OnUserChatEvent(std::shared_ptr<UserChatEvent> event);
    };
} // namespace chattere