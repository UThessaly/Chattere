#include "packet.hpp"

namespace chattere::protocol
{
    std::pair<int, chattere::protocol::Packet>
    packet_from_buffer(std::vector<std::uint8_t> *b)
    {
        std::vector<std::uint8_t> &buffer = *b;

        if (buffer.size() < 4)
        {
            chattere::protocol::Packet p;
            return std::make_pair(0, p);
        }

        uint32_t size = 0;

        size |= ((buffer[0] << (8 * 0)));
        size |= ((buffer[1] << (8 * 1)));
        size |= ((buffer[2] << (8 * 2)));
        size |= ((buffer[3] << (8 * 3)));

        if (size > 0 && buffer.size() >= size + 4)
        {
            std::vector<std::uint8_t> buf(size);

            std::copy(buffer.begin() + 4, buffer.begin() + 4 + size, buf.begin());

            chattere::protocol::Packet packet;

            packet.ParseFromArray(&buf[0], size);
            return std::make_pair(size + 4, packet);
        }
        else
        {
            chattere::protocol::Packet p;
            return std::make_pair(0, p);
        }
    }

    std::pair<int, std::vector<chattere::protocol::Packet>> packets_from_buffer(std::vector<std::uint8_t> *b)
    {
        std::vector<std::uint8_t> &buffer = *b;
        std::vector<chattere::protocol::Packet> packets;
        int readLength = 0;

        while (true)
        {
            if (buffer.size() <= readLength)
            {
                return std::pair(readLength, packets);
            }
            std::vector<std::uint8_t> packetBuf({buffer.begin() + readLength, buffer.end()});
            auto result = packet_from_buffer(&packetBuf);
            if (result.first > 0)
            {
                packets.push_back(result.second);
                readLength += result.first;
            }
            else
            {
                return std::pair(readLength + result.first, packets);
            }
        }

        return std::pair(0, std::vector<chattere::protocol::Packet>{});
    }

    std::vector<std::uint8_t> packet_to_buffer(Packet *packet)
    {
        std::string data;
        packet->SerializeToString(&data);
        auto size = data.size();

        std::vector<std::uint8_t> buffer;

        buffer.push_back((size >> (8 * 0)) & 0xFF);
        buffer.push_back((size >> (8 * 1)) & 0xFF);
        buffer.push_back((size >> (8 * 2)) & 0xFF);
        buffer.push_back((size >> (8 * 3)) & 0xFF);

        buffer.insert(buffer.end(), data.begin(), data.end());

        return buffer;
    }
} // namespace chattere::protocol