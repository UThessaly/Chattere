#pragma once

#include <functional>
#include <sol/sol.hpp>
#include <vector>

namespace chattere::plugins
{
    class PluginManager
    {
    public:
        typedef std::function<void(sol::state &lua)> BindType;

    public:
        static void SBind(BindType bind)
        {
            s_binds.push_back(bind);
        }

        void Bind(BindType bind)
        {
            bind(m_lua);
        }

    private:
        sol::state m_lua;

        static std::vector<std::function<void(sol::state &lua)>> s_binds;
    };
} // namespace chattere::plugins