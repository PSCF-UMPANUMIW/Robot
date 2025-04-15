#pragma once

#include <cstdint>

enum class PacketType : uint8_t
{
    UNSPECIFIED = 0,
    LIDAR,
    LOG,
    SENSORS,
};