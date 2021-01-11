#include "plugins.hpp"
#include "events.hpp"

#include "server.hpp"
#include "user.hpp"
#include "user_command_sender.hpp"
#include "lua_yaml.hpp"
#include <iostream>
#include <fstream>

namespace chattere::plugins
{
    Plugin::Plugin(const std::string &name) : m_name(name)
    {
        // m_lua.open_libraries();
    }

    const std::string &Plugin::GetName()
    {
        return m_name;
    }

    void Plugin::RegisterEventListener(const std::string &event, sol::function listener)
    {
        spdlog::info("Registered Event {}", event);
        m_event_listeners[event].push_back(listener);
    }

    // const std::string Plugin::GetFile(const std::string &path_str)
    // {
    //     auto path = m_data_path / path_str;

    //     std::ifstream file(path.string());

    //     if (!file)
    //         return "";

    //     std::string content((std::istreambuf_iterator<char>(file)),
    //                         (std::istreambuf_iterator<char>()));
    // }

    PluginManager::PluginManager(Server *server)
    {
        auto &lua = m_lua;
        lua.open_libraries();

        auto plugin_type = lua.new_usertype<Plugin>("Plugin",
                                                    sol::constructors<Plugin(std::string)>());

        plugin_type["name"] = &Plugin::m_name;
        plugin_type["on"] = &Plugin::RegisterEventListener;

        // lua["command"] = [&](const std::string &command, const sol::function &func) {
        //     spdlog::info("Registered Command");
        //     server->RegisterCommand(command, func);
        // };

        plugin_type["file"] = [&](const Plugin &plugin, const std::string &path_str) {
            auto path = this->m_plugin_data_path / plugin.m_name / path_str;

            spdlog::info("Opened File: {}", path.string());
            std::ifstream file(path.string());

            if (!file)
                return std::string("");

            std::string content((std::istreambuf_iterator<char>(file)),
                                (std::istreambuf_iterator<char>()));

            return content;
        };

        plugin_type["read_json"] = [&](const Plugin &plugin, const std::string &path_str) {
            auto path = this->m_plugin_data_path / plugin.m_name / path_str;

            spdlog::info("Opened File: {}", path.string());
            std::ifstream file(path.string());

            if (!file)
                return std::string("{}");

            std::string content((std::istreambuf_iterator<char>(file)),
                                (std::istreambuf_iterator<char>()));

            return content;
        };

        plugin_type["save_file"] = [&](const Plugin &plugin, const std::string &path_str, const std::string &content) {
            auto path = this->m_plugin_data_path / plugin.m_name / path_str;
            spdlog::info("saving {}", path.string());
            std::filesystem::create_directories(path.parent_path());

            std::ofstream file(path.string(), std::ios::trunc);
            file << content;
        };

        // plugin_type["save"] = plugin_type["save_file"];

        plugin_type["append_file"] = [&](const Plugin &plugin, const std::string &path_str, const std::string &content) {
            auto path = this->m_plugin_data_path / plugin.m_name / path_str;
            std::filesystem::create_directories(path.parent_path());

            std::ofstream file(path.string(), std::ios::app);
            file << content;
        };

        plugin_type["register"] = [&](Plugin &plugin) {
            spdlog::info("registered plugin");
            m_plugins.push_back(plugin);
        };

        plugin_type["plugin_manager"] = [&]() {
            return this;
        };

        auto plugin_manager_type = lua.new_usertype<PluginManager>("PluginManager", sol::no_construction());
        plugin_manager_type["get_plugins"] = [&]() {
            return m_plugins;
        };

        plugin_manager_type["server"] = &PluginManager::GetServer;

        lua["register_plugin"] = [&](Plugin &plugin) {
            spdlog::info("registered plugin");
            m_plugins.push_back(plugin);
        };

        auto server_type = lua.new_usertype<Server>("Server", sol::no_construction());
        server_type["logger"] = &Server::GetConsoleLogger;

        auto logger_type = lua.new_usertype<spdlog::logger>("Logger", sol::no_construction());
        logger_type["info"] = [](const std::string &message) {
            spdlog::info(message);
        };

        auto basic_event_type = lua.new_usertype<BasicEvent>("BasicEvent", sol::no_construction());
        basic_event_type["set_canceled"] = &BasicEvent::SetCanceled;

        auto basic_user_event_type = lua.new_usertype<UserBasicEvent>("BasicUserEvent", sol::no_construction(),
                                                                      sol::base_classes, sol::bases<BasicEvent>());
        basic_user_event_type["get_user"] = &UserBasicEvent::GetUser;
        basic_user_event_type["get_client"] = &UserBasicEvent::GetClient;

        auto user_type = lua.new_usertype<User>("user", sol::no_construction(),
                                                sol::base_classes, sol::bases<CommandSender>());
        user_type["get_name"] = &User::GetName;

        auto user_chat_event_type = lua.new_usertype<UserChatEvent>("UserChatEvent", sol::no_construction(),
                                                                    sol::base_classes, sol::bases<UserBasicEvent, BasicEvent>());
        user_chat_event_type["get_message"] = &UserChatEvent::GetMessage;
        user_chat_event_type["set_message"] = &UserChatEvent::SetMessage;
        // user_chat_event_type["get_user"] = &UserChatEvent::GetUser;

        user_chat_event_type["get_format"] = &UserChatEvent::GetFormat;
        user_chat_event_type["set_format"] = &UserChatEvent::SetFormat;

        auto command_event_type = lua.new_usertype<CommandEvent>("CommandEvent", sol::no_construction(), sol::base_classes, sol::bases<BasicEvent>());

        command_event_type["get_command"] = &CommandEvent::GetCommand;
        command_event_type["set_command"] = &CommandEvent::SetCommand;
        command_event_type["get_sender"] = &CommandEvent::GetCommandSender;
        command_event_type["get_command_args"] = &CommandEvent::GetArgsArray;
        command_event_type["set_command_args"] = &CommandEvent::SetCommandArgs;
        command_event_type["get_full_command"] = &CommandEvent::GetFullCommand;
        command_event_type["set_command_message"] = &CommandEvent::SetCommandMessage;

        auto sender_type = lua.new_usertype<CommandSender>("CommandSender", sol::no_construction());
        sender_type["get_name"] = &CommandSender::GetName;
        sender_type["get_server"] = &CommandSender::GetServer;
        sender_type["send_message"] = &CommandSender::SendMessage;
        sender_type["is_user"] = &CommandSender::IsUser;
        sender_type["is_console"] = &CommandSender::IsConsole;
    }

    Server *PluginManager::GetServer()
    {
        return m_server;
    }

    void PluginManager::RegisterPlugin(Plugin &plugin)
    {
        spdlog::info("registered plugin");
        m_plugins.push_back(plugin);
    }

    sol::state &PluginManager::GetLua()
    {
        return m_lua;
    }

    std::vector<sol::function> PluginManager::EmitFunctions(const std::string &event)
    {
        std::vector<sol::function> functions;
        for (auto &plugin : m_plugins)
            if (plugin.m_event_listeners.count(event) > 0)
                for (auto &[event, funcs] : plugin.m_event_listeners)
                    functions.insert(functions.end(), funcs.begin(), funcs.end());

        return functions;
    }

    void PluginManager::LoadPlugins(const std::filesystem::path &plugins_path)
    {
        std::filesystem::create_directories(plugins_path);
        m_plugins_path = plugins_path;
        for (auto &plugin_dir : std::filesystem::directory_iterator(plugins_path))
        {
            if (!plugin_dir.is_directory())
                continue;

            if (plugin_dir.path().stem() == "Data")
                continue;

            auto entry = plugin_dir.path() / "plugin.lua";
            std::ifstream plugin(entry.string());

            if (!plugin)
                continue;

            std::string content((std::istreambuf_iterator<char>(plugin)),
                                (std::istreambuf_iterator<char>()));

            m_lua.script(content);
        }
    }

    void PluginManager::LoadLibs(const std::filesystem::path &libs_path)
    {
        std::filesystem::create_directories(libs_path);
        m_libs_path = libs_path;
        for (auto &file : std::filesystem::directory_iterator(libs_path))
        {
            if (!file.is_regular_file())
                continue;

            std::ifstream lib_stream(file.path().string());
            std::string content((std::istreambuf_iterator<char>(lib_stream)),
                                (std::istreambuf_iterator<char>()));

            m_lua.require_script(file.path().stem().string(), content);
        }
    }

    void PluginManager::SetPluginDataPath(const std::filesystem::path &data_path)
    {
        std::filesystem::create_directories(data_path);
        m_plugin_data_path = data_path;
    }
    // sol::state &Plugin::GetVM()
    // {
    //     return m_lua;
    // }
} // namespace chattere::plugins
