#include "user.hpp"

namespace chattere
{
    User::User(Server *server, std::shared_ptr<protocol::User> user) : m_server(server), m_user(user)
    {
    }

    std::int64_t User::GetId()
    {
        return m_user->id();
    }

    void User::SendMessage(const std::string &message)
    {
        protocol::Packet packet;
        auto basic_chat_event = new protocol::BasicChatEvent();
        basic_chat_event->set_message(message);
        packet.set_allocated_basic_chat_event(basic_chat_event);
        auto encoded = protocol::packet_to_buffer(&packet);
        for (auto &socket : m_sockets)
        {
            socket->GetSocket()->Send(encoded);
        }
    }

    // void User::SendMessage(const std::shared_ptr<net::ClientSocket> socket, const std::string &message)
    // {
    // }

    void User::AddClientSocket(std::shared_ptr<net::ClientSocket> client)
    {
        m_sockets.push_back(client);
    }

    bool User::IsUser() const
    {
        return true;
    }

    bool User::IsConsole() const
    {
        return false;
    }

    const std::string User::GetName() const
    {
        return m_user->username();
    }

    Server *User::GetServer() const
    {
        return m_server;
    }
} // namespace chattere
