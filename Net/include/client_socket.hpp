#pragma once

#include "socket.hpp"
#include <memory>
#include <mutex>

namespace chattere::net
{
    class ServerSocket;

    /**
     * @brief Client Socket for clients that connect from ServerSocket
     * 
     */
    class ClientSocket
    {
    public:
        /**
         * @brief Construct a new Client Socket object
         * 
         * @param socket The Client's Socket
         */
        ClientSocket(std::int64_t id, std::shared_ptr<Socket> socket);

        friend ServerSocket;

        /**
         * @brief Recv Buffer.
         * 
         * Contains all the data from Recv
         */

        std::int64_t GetId() const;

        std::shared_ptr<Socket> GetSocket();
        std::mutex &GetMutex();
        std::mutex m_mutex;
        std::vector<std::uint8_t> m_read_buffer;

    private:
        std::shared_ptr<Socket> m_socket;
        std::int64_t m_id = -1;
    };
} // namespace chattere::net