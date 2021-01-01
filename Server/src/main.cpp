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

static constexpr char USAGE[] =
    R"(Chattere Server
    Usage:
      server [options]
      server (-h | --help)
      server --version

    Options:
      -h --help             Show this screen.
      --version             Show version.
      -p --port=<port>      Port to start the server on [default: 20080].
      -d --db=<file>        Database path [default: ./database.db3].   
      -t --threads=<num>    How many threads to be used [default: 1].  
      -e -ethreads=<num>    If events (OnChat, OnConnection) should be
                            Multithreaded. If the value is set to 0,
                            then the events will not be multithreaded [default: 0].
)";

int main(int argc, char const *argv[])
{
  // YAML::Node node;
  // node["hello"] = "world";

  // YAML::Node node2;
  // node2["one"] = "hello";
  // node2["two"].push_back("one");
  // node2["two"].push_back("two");
  // node2["two"].push_back("three");
  // node2["1"].push_back(1);
  // node2["1"].push_back(2);
  // node2["1"].push_back(3);

  // node["a.b.c.d"] = 1;
  // node["a"]["b"]["c"]["d"] = 1;

  // node["settings"] = node2;

  // node["hi"] = {{"one", 1}, {"two", 2}};

  // YAML::Emitter e;
  // e << node;
  // std::cout << e.c_str() << std::endl;
  // return 0;
  // auto e = std::dynamic_pointer_cast<BasicEvent>(event);

  // e->GetName();

  auto args = docopt::docopt(USAGE, {argv + 1, argv + argc}, true, "Chattere Server 1.0");

  const auto port = args["--port"].isLong() ? static_cast<std::uint16_t>(args["--port"].asLong()) : static_cast<std::uint16_t>(std::atoi(args["--port"].asString().data()));
  // const auto event_threads = args["-e"].isLong() ? static_cast<std::uint16_t>(args["-e"].asLong()) : static_cast<std::uint16_t>(std::atoi(args["-e"].asString().data()));

  const auto &db = args["--db"].asString();
  // const std::uint16_t threads = args["--threads"].isLong() ? static_cast<std::uint16_t>(args["--threads"].asLong()) : static_cast<std::uint16_t>(std::atoi(args["--threads"].asString().data()));
  const int threads = 5;

  chattere::Server server(port);
  server.SetProperty("threads", threads);

  spdlog::info("{}", server.ColorFormat("$b&cHello&r #a&dworld &r"));

  server.GetConsoleLogger()->info("    {}  Server started on port {}", chattere::EMOJIS["smile"], port);
  return 0;

  server.Listen();

  for(auto& future : server.m_futures) {
    future.wait();
  }
  return 0;
}
