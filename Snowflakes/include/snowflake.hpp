#pragma once

#include <cinttypes>

namespace chattere
{
    static std::int64_t EPOCH = 1609187816000;

    struct Snowflake
    {
        /** Only the 41 bits are used */
        std::uint64_t time;

        /** Only 10 bits are used */
        std::uint16_t worker_id = 0;

        /** Only 12 bits are used */
        std::uint16_t sequence_id = 0;
    };

    std::int64_t CreateSnowflake(std::uint16_t worker_id = 0);
} // namespace chattere