#pragma once

#include <string>
#include "server.hpp"
#include <sol/sol.hpp>
#include "events.hpp"
#include <map>
#include <vector>

namespace chattere::plugins
{
    class Plugin
    {
    public:
        Plugin(const std::string &name);

        const std::string &GetName();
        void RegisterEventListener(const std::string &event, sol::function listener);

        template <typename EventType, typename std::enable_if<std::is_base_of<BasicEvent, EventType>::value>::type * = nullptr>
        void Emit(const std::string &event, EventType data)
        {
            for (auto &listener : m_event_listeners)
            {
                listener(data);
            }
        }

        sol::state &GetVM();

    private:
        std::map<std::string, std::vector<sol::function>> m_event_listeners;
        sol::state m_lua;
        std::string m_name;
    };

    class PluginManager
    {
    public:
        PluginManager(Server *server);

    private:
        Server *m_server;
        std::vector<Plugin> m_plugins;
    };
} // namespace chattere::plugins