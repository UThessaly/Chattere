#pragma once

#include "net.hpp"

#include "client_socket.hpp"
#include <map>
#include <memory>

namespace chattere::net
{
    class ServerSocket
    {
    public:
        ServerSocket(Socket &sock);
        ServerSocket(std::uint16_t port);
        ServerSocket(InetSocketAddress sockAddr);

        std::shared_ptr<ClientSocket> Accept();
        void Bind();
        void Close();
        void Queue(std::size_t queueSize = 5);

        // void SetSocketAddress(std::uint16_t port);
        // void SetSocketAddesss(InetSocketAddress sockAddr);

        void ClientsRecv();

        std::map<long, std::shared_ptr<ClientSocket>> GetClients();

    private:
        Socket m_socket;

        long m_clientIdCounter = 0;
        std::map<long, std::shared_ptr<ClientSocket>> m_clients;
    };
} // namespace chattere::net