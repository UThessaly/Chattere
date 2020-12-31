#include "server_socket.hpp"
#include <spdlog/spdlog.h>

#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>

namespace chattere::net
{
    ServerSocket::ServerSocket(Socket &sock) : m_socket(sock)
    {
        if (fcntl(m_socket.m_socket, F_SETFL, fcntl(m_socket.m_socket, F_GETFL) | O_NONBLOCK) < 0)
        {
            spdlog::error("Error setting Socket to be NonBlocking for server socket {}", m_socket.m_socket);
        }
    }

    ServerSocket::ServerSocket(std::uint16_t port) : m_socket(Socket(port))
    {
        if (fcntl(m_socket.m_socket, F_SETFL, fcntl(m_socket.m_socket, F_GETFL) | O_NONBLOCK) < 0)
        {
            spdlog::error("Error setting Socket to be NonBlocking for server socket {}", m_socket.m_socket);
        }
    }

    ServerSocket::ServerSocket(InetSocketAddress sockAddr) : m_socket(Socket(sockAddr))
    {
        if (fcntl(m_socket.m_socket, F_SETFL, fcntl(m_socket.m_socket, F_GETFL) | O_NONBLOCK) < 0)
        {
            spdlog::error("Error setting Socket to be NonBlocking for server socket {}", m_socket.m_socket);
        }
    }

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

        if (client_socketFd <= 0 && errno == EWOULDBLOCK)
        {
            return nullptr;
        }
        auto id = m_clientIdCounter++;
        if (client_socketFd < 0)
        {
            spdlog::error("Connection Error with client {} with error: ", id, strerror(errno));
        }

        if (fcntl(client_socketFd, F_SETFL, fcntl(client_socketFd, F_GETFL) | O_NONBLOCK) < 0)
        {
            spdlog::error("Error setting Socket to be NonBlocking for client {}", id);
        }

        InetSocketAddress addr(cli_addr);

        auto socket = std::make_shared<Socket>(addr, client_socketFd);
        auto client = std::make_shared<ClientSocket>(id, socket);

        // client->m_socket->OnData([&](std::size_t size, std::vector<std::uint8_t> buffer) {
        //     // Reaching a Deadlock, not sure why. Removing the mutex from here
        //     // Just because I can't figure it out why.
        //     // There is a VERY SLIM chance that the program will completely break
        //     // Just restart it when and if it even does
        //     // {
        //         // std::lock_guard<std::mutex> guard(client->m_mutex);
        //         client->m_read_buffer.insert(client->m_read_buffer.end(), buffer.begin(), buffer.begin() + size);
        //     // }
        // });
        m_clients.insert_or_assign(id, client);

        return client;
    }

    void ServerSocket::ClientsRecv()
    {
        for (auto &[id, client] : m_clients)
        {
            auto &[err, size, data] = client->m_socket->Recv();
            if (size <= 0)
                continue;

            client->m_read_buffer.insert(client->m_read_buffer.begin(), data.begin(), data.begin() + size);
        }
    }

    std::map<long, std::shared_ptr<ClientSocket>> ServerSocket::GetClients()
    {
        return m_clients;
    }
} // namespace chattere::net