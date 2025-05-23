#pragma once

#include <EspNowSizeLimits.hpp>
#include <LidarMeasurement.hpp>

struct PayloadLidar
{
    static constexpr size_t MEASUREMENTS_PER_PACKET = MAX_PAYLOAD_SIZE / sizeof(LidarMeasurement);

    LidarMeasurement measurements[MEASUREMENTS_PER_PACKET];
};