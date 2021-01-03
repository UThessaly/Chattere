#include "plugins.hpp"

namespace chattere::plugins
{
    Plugin::Plugin(const std::string &name) : m_name(name)
    {
        m_lua.open_libraries();
    }

    const std::string &Plugin::GetName()
    {
        return m_name;
    }

    void Plugin::RegisterEventListener(const std::string &event, sol::function listener)
    {
        m_event_listeners[event].push_back(listener);
    }

    sol::state &Plugin::GetVM()
    {
        return m_lua;
    }
} // namespace chattere::plugins
