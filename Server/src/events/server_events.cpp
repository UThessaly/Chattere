#include "server_events.hpp"
#include <sstream>
#include "emoji.hpp"
#include "user.hpp"
#include "events.hpp"
#include "server.hpp"

namespace chattere
{

    ServerEventHandler::ServerEventHandler(Server *server) : m_server(server), m_listeners({})
    {
    }

    void ServerEventHandler::RegisterEventListener(std::shared_ptr<EventListener> listener)
    {
        m_listeners.push_back(listener);
    }

    void ServerEventHandler::EmitUserChatEvent(std::shared_ptr<net::ClientSocket> client, std::shared_ptr<User> user, std::string &message)
    {
        auto event = std::make_shared<UserChatEvent>(m_server, user, client, message);
        for (const auto &listener : m_listeners)
        {
            listener->OnUserChatEvent(event);
            if (event->IsCanceled())
                return;
        }

        auto logger = m_server->GetConsoleLogger();
        auto msg_format = event->GetFormat();

        if (auto index = msg_format.find("$0"); index != std::string::npos)
        {
            msg_format.replace(index, 2, user->GetName());
        }

        if (auto index = msg_format.find("$1"); index != std::string::npos)
        {
            msg_format.replace(index, 2, message);
        }

        logger->info(msg_format);
    }

    void ServerEventHandler::EmitUserCommandEvent(std::shared_ptr<net::ClientSocket> client, std::shared_ptr<User> user, std::string &message)
    {
        auto event = std::make_shared<UserChatEvent>(m_server, user, client, message);
        for (const auto &listener : m_listeners)
        {
            listener->OnUserChatEvent(event);
            if (event->IsCanceled())
                return;
        }

        auto logger = m_server->GetConsoleLogger();
        auto msg_format = event->GetFormat();

        if (auto index = msg_format.find("$0"); index != std::string::npos)
        {
            msg_format.replace(index, 2, user->GetName());
        }

        if (auto index = msg_format.find("$1"); index != std::string::npos)
        {
            msg_format.replace(index, 2, message);
        }

        logger->info(msg_format);
    }

    void BasicServerEventListener::OnUserChatEvent(std::shared_ptr<UserChatEvent> event)
    {
        std::stringstream ss;
        ss << chattere::EMOJIS["speech_balloon"] << " $1: $0";
        auto str = ss.str();
        event->SetFormat(str);
    }
} // namespace chattere