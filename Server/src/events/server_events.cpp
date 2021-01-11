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

        for (const auto &lua_listener : m_server->GetPluginManager().EmitFunctions("message"))
        {
            lua_listener(event);
            if (event->IsCanceled())
                return;
        }

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
            msg_format.replace(index, 2, event->GetUser()->GetName());
        }

        if (auto index = msg_format.find("$1"); index != std::string::npos)
        {
            msg_format.replace(index, 2, event->GetMessage());
        }

        logger->info(msg_format);

        // Send to all clients
    }

    void ServerEventHandler::EmitCommandEvent(const std::shared_ptr<CommandSender> sender, std::string &command)
    {
        auto event = std::make_shared<CommandEvent>(m_server, sender, command);

        for (const auto &lua_listener : m_server->GetPluginManager().EmitFunctions("command"))
        {
            lua_listener(event);
            if (event->IsCanceled())
                return;
        }

        for (const auto &listener : m_listeners)
        {
            listener->OnCommandEvent(event);
            if (event->IsCanceled())
                return;
        }

        auto executor = m_server->GetCommandExecutor(event->GetCommand());

        // executor(sender, event->GetCommand(), event->GetArgsArray());
        try
        {
            std::get<sol::function>(executor)(sender, event->GetCommand(), event->GetArgsArray());
            return;
        }
        catch (std::bad_variant_access err)
        {
        }

        try
        {
            std::get<std::function<bool(std::shared_ptr<CommandSender>, const std::string &, const std::vector<std::string>)>>(executor)(sender, event->GetCommand(), event->GetArgsArray());
            return;
        }
        catch (std::bad_variant_access err)
        {
        }

    }

    void BasicServerEventListener::OnUserChatEvent(std::shared_ptr<UserChatEvent> event)
    {
        std::stringstream ss;
        ss << chattere::EMOJIS["speech_balloon"] << " $0: $1";
        auto str = ss.str();
        event->SetFormat(str);
    }

    void BasicServerEventListener::OnCommandEvent(std::shared_ptr<CommandEvent> event)
    {
        spdlog::critical("Received command {}", event->GetCommand());
    }
} // namespace chattere