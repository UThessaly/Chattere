#pragma once

#include "inet_socket_address.hpp"
#include <vector>
#include <functional>
#include <fpu_control.h>

namespace chattere::net
{
    /**
     * @brief The Socket State
     * 
     */
    enum class SocketState
    {
        eDisconnected,
        eDisconnecting,
        eConnecting,
        eConnected,
        eUnknown
    };

    /**
 * @brief The recv() error
 * 
 */
    enum class RecvError
    {
        eNone = 0,
        eWouldBlock = 1,
        eUnknown = 2,
    };

    class ServerSocket;
    class SocketChannel;

    /**
     * @brief Non Blocking Socket
     */
    class Socket
    {
    public:
        typedef int socket_t;

        /**
         * @brief Construct a new Socket object
         * 
         * @param address The Address to Listen to
         * @param port The Port to listen to
         */
        Socket(std::string address, std::uint16_t port);

        /**
         * @brief Construct a new Socket object
         * 
         * @param bindpoint The Address to Listen to
         * @param socket The Socket Descriptor
         */
        Socket(InetSocketAddress bindpoint, socket_t socket);

        /**
         * @brief Construct a new Socket object
         * 
         * @param bindpoint The Address to Listen to
         */
        Socket(InetSocketAddress bindpoint);

        friend ServerSocket;
        friend SocketChannel;

        /**
         * @brief Function to call when Recv is called
         * 
         */
        void OnData(std::function<void(const std::size_t, const std::vector<std::uint8_t>)>);

        /**
         * @brief 
         * 
         * @return const std::tuple<RecvError, int, std::vector<std::uint8_t>> 
         */
        const std::tuple<RecvError, int, std::vector<std::uint8_t>> Recv(std::size_t bytes = 1024 * 128) const;

        void Send(std::vector<std::uint8_t> &buffer) const;
        void Send(std::vector<std::uint8_t> &&buffer) const;

        InetSocketAddress &GetBindpoint();

        void Connect();

        const socket_t GetSocketFD() const;

    private:
        std::vector<std::function<void(const std::size_t, const std::vector<std::uint8_t>)>> m_onDataHandlers;

        InetSocketAddress m_bindpoint;
        socket_t m_socket;
    };
} // namespace chattere::net