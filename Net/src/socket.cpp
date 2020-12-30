#include "socket.hpp"
#include "inet_socket_address.hpp"
#include <unistd.h>
#include <spdlog/spdlog.h>

namespace chattere::net
{
    Socket::Socket(std::string address, std::uint16_t port) : Socket(InetSocketAddress(address, port))
    {
    }

    Socket::Socket(InetSocketAddress bindpoint, socket_t socket) : m_bindpoint(bindpoint),
                                                                   m_socket(socket)
    {
    }

    Socket::Socket(InetSocketAddress bindpoint) : Socket(bindpoint, socket(AF_INET, SOCK_STREAM, 0))
    {
    }

    /**
     * TODO: Change std::make_tuple() to return the size that was read, instead of the entire buffer 
     */
    const std::tuple<RecvError, int, std::vector<std::uint8_t>> Socket::Recv(std::size_t bytes) const
    {
        std::vector<std::uint8_t> buffer(bytes);

        int res = recv(m_socket, &buffer[0], bytes, MSG_DONTWAIT);

        if (res > 0)
        {
            spdlog::debug("[{} ->] Received {} bytes", m_socket, res);

            for (const auto &func : m_onDataHandlers)
                func(res, buffer);

            return std::make_tuple(RecvError::eNone, res, buffer);
        }

        // spdlog::debug("[{} ->] Could not read from socket: {} {}", m_socket, res, strerror(errno));

        switch (res)
        {
        case EAGAIN:
            return std::make_tuple(RecvError::eWouldBlock, 0, buffer);
        default:
            return std::make_tuple(RecvError::eUnknown, 0, buffer);
        }
    }

    void Socket::OnData(std::function<void(const std::size_t, const std::vector<std::uint8_t>)> func)
    {
        m_onDataHandlers.push_back(func);
    }

    InetSocketAddress &Socket::GetBindpoint()
    {
        return m_bindpoint;
    }

    const Socket::socket_t Socket::GetSocketFD() const
    {
        return m_socket;
    }

    void Socket::Connect()
    {
        if (connect(m_socket, (sockaddr *)&m_bindpoint.GetSockAddr(), sizeof(m_bindpoint.GetSockAddr())) < 0)
        {
            spdlog::critical("Error when connecting to remote server: {}", strerror(errno));
        }
    }

    void Socket::Send(std::vector<uint8_t> &buffer) const
    {
        write(m_socket, &buffer[0], buffer.size());
    }

    void Socket::Send(std::vector<uint8_t> &&buffer) const
    {
        write(m_socket, &buffer[0], buffer.size());
    }
} // namespace chattere::net