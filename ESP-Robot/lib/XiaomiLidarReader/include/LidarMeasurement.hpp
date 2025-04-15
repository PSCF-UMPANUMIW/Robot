#pragma once

#include <cstdint>

struct LidarMeasurement
{
    uint16_t angle;
    uint16_t speed;
    uint16_t distance[4];
    // uint16_t quality[4]; ?
};