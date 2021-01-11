#pragma once

#include <string>
#include <sol/sol.hpp>
#include "events.hpp"
#include <map>
#include <vector>
#include <functional>
#include <filesystem>

namespace chattere::plugins
{
    class PluginManager;
    class Plugin
    {
    public:
        Plugin(const std::string &name);

        const std::string &GetName();
        void RegisterEventListener(const std::string &event, sol::function listener);

        friend PluginManager;
        // const std::string GetFile(const std::string &path);

    private:
        std::string m_name;
        std::filesystem::path m_data_path;

        std::map<std::string, std::vector<sol::function>> m_event_listeners;
    };

    class PluginManager
    {
    public:
        PluginManager(Server *server);

        void RegisterPlugin(Plugin &plugin);

        sol::state &GetLua();

        template <typename T>
        void GlobalEmit(const std::string &event, T &data)
        {
            for (auto &plugin : m_plugins)
            {
                if (plugin.m_event_listeners.count(event) == 0)
                    continue;

                for (auto &listener : plugin.m_event_listeners[event])
                {
                    listener(data);
                }
            }
        }

        std::vector<sol::function> EmitFunctions(const std::string &event);
        void LoadLibs(const std::filesystem::path &path);

        Server *GetServer();
        void LoadPlugins(const std::filesystem::path &plugins_path);

        void SetPluginDataPath(const std::filesystem::path &data_path);

    private:
        std::map<std::string, std::vector<sol::function>> m_event_listeners;
        sol::state m_lua;
        Server *m_server;
        std::filesystem::path m_plugins_path;
        std::filesystem::path m_libs_path;
        std::filesystem::path m_plugin_data_path;
        std::vector<Plugin> m_plugins;
    };
} // namespace chattere::plugins