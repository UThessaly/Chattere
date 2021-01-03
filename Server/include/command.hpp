#pragma once

#include <functional>
#include <vector>
#include <string>
#include <memory>

namespace chattere
{
    class CommandSender;
    typedef std::function<bool(std::shared_ptr<CommandSender>, const std::string &, const std::vector<std::string>)> CommandExecutor;
} // namespace chattere