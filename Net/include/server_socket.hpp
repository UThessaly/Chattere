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
        /**
         * @brief Construct a new Server Socket object
         * 
         * @param sock Socket Descriptor
         */
        ServerSocket(Socket &sock);

        /**
         * @brief Construct a new Server Socket object
         * 
         * @param port The port to listen on
         */
        ServerSocket(std::uint16_t port);

        /**
         * @brief Construct a new Server Socket object
         * 
         * @param sockAddr The Address and Port to bind to
         */
        ServerSocket(InetSocketAddress sockAddr);

        /**
         * @brief Accepts a client.
         * 
         * @return nullptr - Error / Would Block
         * 
         * @return std::shared_ptr<ClientSocket>
         */
        std::shared_ptr<ClientSocket> Accept();

        /**
         * @brief Binds the socket 
         */
        void Bind();

        /**
         * @brief Closes the Socket 
         */
        void Close();

        /**
         * @brief How many clients will be queued up, before rejecting them
         * 
         * @param queueSize 
         */
        void Queue(std::size_t queueSize = 5);

        // void SetSocketAddress(std::uint16_t port);
        // void SetSocketAddesss(InetSocketAddress sockAddr);

        /**
         * @brief Uses Recv in all clients
         */
        void ClientsRecv();

        /**
         * @brief Get the Clients list
         * 
         * @return std::map<long, std::shared_ptr<ClientSocket>> 
         */
        std::map<long, std::shared_ptr<ClientSocket>> GetClients();

    private:
        /**
         * @brief The Socket the server listens on
         */
        Socket m_socket;

        /**
         * @brief Client Id.
         * 
         * Bound to change
         */
        long m_clientIdCounter = 0;

        /**
         * @brief The Clients 
         */
        std::map<long, std::shared_ptr<ClientSocket>> m_clients;
    };
} // namespace chattere::net