#pragma once

#include <cstdint>

struct LidarMeasurement
{
    uint16_t startAngle;
    uint16_t distance[4];
    // uint16_t quality[4]; ?
};