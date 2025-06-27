#pragma once

#include <cstdint>

enum class PacketID : uint8_t
{
    UNSPECIFIED = 0,
    LIDAR,
    LOG,
    SENSORS,
    MOVE_COMMAND,
    MOTOR_CONFIG,
    LIDAR_CONFIG,
};