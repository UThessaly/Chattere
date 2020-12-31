#pragma once

#include <string>
#include <vector>

namespace chattere
{
    class ServerOperator
    {
    public:
        bool op = false;
    };

    class Permissible : public ServerOperator
    {
        bool HasPermission(const std::string &permission);
        void AddPermission(const std::string &permission);
        void RemovePermission(const std::string &permission);

    private:
        std::vector<std::string> m_permissions{};
    };
} // namespace chattere