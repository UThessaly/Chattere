#include "lua_yaml.hpp"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <spdlog/spdlog.h>

namespace chattere::plugins
{
    LuaYaml::LuaYaml(const std::string &input_yaml) : m_base(YAML::Load(input_yaml))
    {
    }

    LuaYaml::LuaYaml(YAML::Node &child) : m_base(child)
    {
    }

    const std::string LuaYaml::GetString(const std::string &key)
    {
        auto node = GetNode(key);
        return node.as<std::string>();
    }

    const double LuaYaml::GetNumber(const std::string &key)
    {
        auto node = GetNode(key);
        return node.as<double>();
    }

    YAML::Node LuaYaml::GetNode(const std::string &key)
    {
        std::vector<std::string> args;
        boost::split(args, key, boost::is_any_of("."), boost::token_compress_on);

        const auto e = m_base;

        YAML::Node result = GetNode();

        for (const auto &key_arg : args)
        {
            spdlog::critical("Reading {} with str {}: {}", key_arg, result[key_arg].IsScalar(), result[key_arg].IsScalar() ? result[key_arg].as<std::string>() : "unknown");
            result = result[key_arg];
        }

        return std::move(result);
    }

    const YAML::Node LuaYaml::GetNode()
    {
        return m_base;
    }

    // LuaYaml LuaYaml::Get(const std::string &key)
    // {
    //     return LuaYaml(std::move(GetNode(key));
    // }

    void LuaYaml::SetString(const std::string &key, const std::string &value)
    {
        auto node = GetNode(key);
        node = value;
    }

    void LuaYaml::SetNumber(const std::string &key, const double value)
    {
        auto node = GetNode(key);
        node = value;
    }

} // namespace chattere::plugins