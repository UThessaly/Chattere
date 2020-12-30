#include "client_socket.hpp"

namespace chattere::net
{
    ClientSocket::ClientSocket(std::int64_t id, std::shared_ptr<Socket> socket) : m_id(id), m_socket(socket)
    {
    }

    std::shared_ptr<Socket> ClientSocket::GetSocket()
    {
        return m_socket;
    }

    std::int64_t ClientSocket::GetId() const
    {
        return m_id;
    }
} // namespace chattere::net