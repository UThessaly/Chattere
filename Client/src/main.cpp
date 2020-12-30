#include <iostream>
#include "net.hpp"
#include "inet_socket_address.hpp"
#include "packet.hpp"
#include <thread>
#include <chrono>
#include "emoji.hpp"
#include <spdlog/spdlog.h>
#include <future>

int main(int argc, char const *argv[])
{
    chattere::net::Socket sock(chattere::net::InetSocketAddress("0.0.0.0", 25561));

    sock.Connect();

    using namespace chattere::protocol;

    Packet packet;
    AuthRequestPacket *request = new AuthRequestPacket();
    request->set_username("kozy");
    request->set_password("12345");

    packet.set_allocated_auth_request(request);

    auto encoded = packet_to_buffer(&packet);

    std::vector<std::uint8_t> buffer;
    sock.OnData([&](std::size_t length, std::vector<std::uint8_t> data) {
        buffer.insert(buffer.begin(), data.begin(), data.begin() + length);
    });
    sock.Send(encoded);

    std::cout << "#general: ";
    std::string in;
    std::getline(std::cin, in);
    auto f = std::async(std::launch::async, [&]() {
        while (true)
        {
            sock.Recv();

            const auto &[size, packets] = chattere::protocol::packets_from_buffer(&buffer);
            if (size > 0)
            {
                buffer.erase(buffer.begin(), buffer.begin() + size);
                for (auto &incomingPacket : packets)
                {
                    spdlog::info("Received packet {}", incomingPacket.data_case());
                }
            }
        }
    });
    while (true)
    {

        std::string msg;
        std::getline(std::cin, msg);

        auto chat = new chattere::protocol::ChatPacket();
        chat->set_channel_id(123);
        chat->set_content(msg);

        packet.set_allocated_chat(chat);
        sock.Send(chattere::protocol::packet_to_buffer(&packet));
    }
    f.wait();
    // std::cout << "Hello, World" << std::endl;
    return 0;
}
