#include "server_socket.hpp"
#include <spdlog/spdlog.h>

#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>

namespace chattere::net
{
    ServerSocket::ServerSocket(Socket &sock) : m_socket(sock)
    {
    }

    ServerSocket::ServerSocket(std::uint16_t port) : m_socket(Socket(port))
    {
    }

    ServerSocket::ServerSocket(InetSocketAddress sockAddr) : m_socket(Socket(sockAddr)) {}

    void ServerSocket::Bind()
    {
        const auto sock_addr = m_socket.GetBindpoint().GetSockAddr();
        if (bind(m_socket.GetSocketFD(), (struct sockaddr *)&sock_addr, sizeof(sock_addr)) != 0)
        {
            spdlog::critical("Could not bind: {}", strerror(errno));
            exit(1);
        }
    }

    void ServerSocket::Queue(std::size_t queueSize)
    {
        if (listen(m_socket.GetSocketFD(), queueSize) != 0)
        {
            spdlog::critical("Could not queue up clients: ", strerror(errno));
            exit(1);
        }
    }

    std::shared_ptr<ClientSocket> ServerSocket::Accept()
    {
        sockaddr_in cli_addr;
        int client_socketFd, portno;
        socklen_t clilen = sizeof(cli_addr);
        // std::cout << "Waiting... " << m_socket.GetSocketFd() << '\n';
        client_socketFd = accept(m_socket.GetSocketFD(), (struct sockaddr *)&cli_addr, &clilen);

        auto id = m_clientIdCounter++;

        if (client_socketFd < 0)
        {
            spdlog::error("Connection Error with client {}", id);
        }

        if (fcntl(client_socketFd, F_SETFL, fcntl(client_socketFd, F_GETFL) | O_NONBLOCK) < 0)
        {
            spdlog::error("Error setting Socket to be NonBlocking for client {}", id);
        }

        InetSocketAddress addr(cli_addr);

        auto socket = std::make_shared<Socket>(addr, client_socketFd);
        auto client = std::make_shared<ClientSocket>(id, socket);

        client->m_socket->OnData([&](std::size_t size, std::vector<std::uint8_t> buffer) {
            client->m_read_buffer.insert(client->m_read_buffer.end(), buffer.begin(), buffer.begin() + size);
        });

        m_clients.insert_or_assign(id, client);

        return client;
    }

    void ServerSocket::ClientsRecv()
    {
        for (auto &[id, client] : m_clients)
        {
            client->m_socket->Recv();
        }
    }

    std::map<long, std::shared_ptr<ClientSocket>> ServerSocket::GetClients()
    {
        return m_clients;
    }
} // namespace chattere::net