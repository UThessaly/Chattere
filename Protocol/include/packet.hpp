#include "packet.pb.h"
#include "database.pb.h"

namespace chattere::protocol
{
    std::pair<int, chattere::protocol::Packet>
    packet_from_buffer(std::vector<std::uint8_t> *b);

    std::pair<int, std::vector<chattere::protocol::Packet>> packets_from_buffer(std::vector<std::uint8_t> *b);

    std::vector<std::uint8_t> packet_to_buffer(Packet *packet);
} // namespace chattere::protocol