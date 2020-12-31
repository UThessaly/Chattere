#pragma once

#include "actions.hpp"
#include "on_user_message.hpp"
#include "server.hpp"

namespace chattere
{
    class BasicEvent
    {
    public:
        BasicEvent(Server *server, const std::string &name);
        std::string GetName();

    protected:
        Server *server;
        const std::string m_name;
    };

    class OnUserMessage : public BasicEvent
    {
    public:
        OnUserMessage(Server *server);

        void SetMessage(std::string &message);
    };

    class OnUserCommand : public BasicEvent
    {
    };

} // namespace chattere