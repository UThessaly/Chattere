#include "server.hpp"
#include <spdlog/spdlog.h>
#include "emoji.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "snowflake.hpp"

#include <SQLiteCpp/SQLiteCpp.h>

int main(int argc, char const *argv[])
{
    chattere::Server server(25561);

    // auto user = server.GetDatabase().GetUserById(12355);

    // if (user)
    // {
    //     server.GetConsoleLogger()->info("User: {}, {}", user->username(), user->id());
    // }

    // auto channels = server.GetDatabase().GetChannels();

    // for (auto &channel : channels)
    // {
    //     spdlog::info("{} {}", channel->id(), channel->channel_case());
    // }

    server.GetConsoleLogger()->info("    {}  Server started on port 25565", chattere::EMOJIS["smile"]);
    // server.GetDatabase().CreateUser("One", "hello");
    server.Listen();
    return 0;
}
