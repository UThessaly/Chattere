#include "snowflake.hpp"
#include <atomic>
#include <chrono>

namespace chattere
{
    std::atomic<std::uint16_t> sequence = 0;
    constexpr auto sequence_mask = 0b111111111111;
    decltype(EPOCH) last_timestamp = 0;

    std::int64_t CreateSnowflake(std::uint16_t worker_id)
    {
        // Does not check what happens if more than 2^12 ids are generated in a singe ms
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        sequence = (sequence + 1) & sequence_mask;

        return ((ms - EPOCH) << 41) |
               (worker_id << 10) | sequence;
    }
} // namespace chattere