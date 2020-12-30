#include <spdlog/spdlog.h>
#include "inet_socket_address.hpp"
#include <arpa/inet.h>


namespace chattere::net {
    InetSocketAddress::InetSocketAddress(std::string address, std::uint16_t port) /*: m_address(address), m_port(port) */ {
        spdlog::debug("Creating InetSocketAddress for Address {} and Port {}", address, port);
        m_addr.sin_family = AF_INET;
        m_addr.sin_port = htons(port);
        m_addr.sin_addr.s_addr = inet_addr(address.c_str());
    }

    InetSocketAddress::InetSocketAddress(std::uint16_t port) /* : m_address("0.0.0.0"), m_port(port) */ {
        spdlog::debug("Creating InetSocketAddress for Address {} and Port {}", "0.0.0.0", port);
        m_addr.sin_family = AF_INET;
        m_addr.sin_port = htons(port);
        m_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    }
    
    InetSocketAddress::InetSocketAddress(sockaddr_in addr) : m_addr(addr) {}

    void InetSocketAddress::setAddress(std::string address) { 
        spdlog::debug("Setting InetSocketAddress Address to {}", address);

        m_addr.sin_addr.s_addr = inet_addr(address.c_str());
    }

    void InetSocketAddress::setPort(std::uint16_t port) {
        spdlog::debug("Setting InetSocketAddress Portt to {}", port);

        m_addr.sin_port = htons(port);
    }

    sockaddr_in& InetSocketAddress::GetSockAddr() {
        return m_addr;
    }
}