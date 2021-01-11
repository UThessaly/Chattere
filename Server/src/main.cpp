#include "server.hpp"
#include <spdlog/spdlog.h>
#include "emoji.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "snowflake.hpp"
#include <docopt/docopt.h>
#include <cstdlib>
#include <cmath>

#include <SQLiteCpp/SQLiteCpp.h>
#include <yaml-cpp/yaml.h>
#include <filesystem>

static constexpr char USAGE[] =
    R"(Chattere Server
    Usage:
      chattere_server [options]
      chattere_server (-h | --help)
      chattere_server --version

    Options:
      -h --help             Show this screen.
      --version             Show version.
      -p --port=<port>      Port to start the server on [default: 20080].
      -d --db=<file>        Database path [default: ./database.db3].   
      -t --threads=<num>    How many threads to be used [default: 1].  
      -e --ethreads=<num>   If events (OnChat, OnConnection) should be
                            Multithreaded. If the value is set to 0,
                            then the events will not be multithreaded [default: 0].
      -l --libs=<dir>       Lua Libraries Directory [default: ./libs].
      -pl --plugins=<dir>   Lua Plugins to Load [default: ./plugins].
      --data=<dir>          Lua Plugin Data directory [default: ./plugins/data]
      
)";

#include <sol/sol.hpp>
#include "plugins.hpp"
#include "packet.hpp"
#include <spdlog/spdlog.h>

int main(int argc, char const *argv[])
{
  auto args = docopt::docopt(USAGE, {argv + 1, argv + argc}, true, "Chattere Server 1.0");

  const auto port = args["--port"].isLong() ? static_cast<std::uint16_t>(args["--port"].asLong()) : static_cast<std::uint16_t>(std::atoi(args["--port"].asString().data()));
  const auto event_threads = args["-e"].isLong() ? static_cast<std::uint16_t>(args["--ethreads"].asLong()) : static_cast<std::uint16_t>(std::atoi(args["--ethreads"].asString().data()));

  const auto &db = args["--db"].asString();
  const auto threads = args["--threads"].isLong() ? static_cast<int>(args["--threads"].asLong()) : static_cast<int>(std::atoi(args["--threads"].asString().data()));

  chattere::Server server(port);

  auto &lua = server.GetPluginManager().GetLua();

  auto lua_libs = args["--libs"].asString();
  auto lua_plugins = args["--plugins"].asString();
  auto lua_plugins_data = args["--data"].asString();

  server.GetPluginManager().SetPluginDataPath(lua_plugins_data);
  server.GetPluginManager().LoadLibs(lua_libs);
  server.GetPluginManager().LoadPlugins(lua_plugins);

  server.SetProperty("threads", threads);

  server.GetConsoleLogger()->info("    {}  Server started on port {}", chattere::EMOJIS["smile"], port);

  server.Listen();

  for (auto &future : server.m_futures)
  {
    future.wait();
  }
  return 1;
}
