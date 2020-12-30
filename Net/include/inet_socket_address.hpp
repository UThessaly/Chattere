#pragma once

#include <string>
#include <cinttypes>

#include <netinet/in.h>

namespace chattere::net
{
    class InetSocketAddress
    {
    public:
        InetSocketAddress();
        InetSocketAddress(sockaddr_in addr);
        InetSocketAddress(std::string address, std::uint16_t port);
        InetSocketAddress(std::uint16_t port);

    public:
        void setAddress(std::string address);

        void setPort(std::uint16_t port);

        sockaddr_in& GetSockAddr();
        // const std::string& GetAddress() const;
        // std::uint16_t GetPort() const;

    private:
        sockaddr_in m_addr = {0};
    };
} // namespace chattere::net