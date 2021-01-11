#pragma once

#include <yaml-cpp/yaml.h>
#include <string>

namespace chattere::plugins
{
    class LuaYaml
    {
    public:
        LuaYaml(const std::string &input_yaml);
        LuaYaml(YAML::Node &child);

        const std::string GetString(const std::string &key);
        void SetString(const std::string &key, const std::string &value);
        const double GetNumber(const std::string &key);
        void SetNumber(const std::string &key, const double value);
        YAML::Node GetNode(const std::string &key);

        LuaYaml Get(const std::string &key);

    private:
        const YAML::Node GetNode();
        YAML::Node m_base;
    };
} // namespace chattere::plugins