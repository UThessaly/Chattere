#pragma once

#include <functional>
#include <vector>
#include <string>
#include <memory>
#include <variant>
#include <sol/sol.hpp>

namespace chattere
{
    class CommandSender;
    class Server;
    typedef std::variant<std::function<bool(std::shared_ptr<CommandSender>, const std::string &, const std::vector<std::string>)>, sol::function> CommandExecutor;
} // namespace chattere