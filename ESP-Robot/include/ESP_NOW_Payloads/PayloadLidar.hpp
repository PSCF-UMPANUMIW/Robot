#pragma once

#include "LidarMeasurement.hpp"
#include "constants.hpp"

struct PayloadLidar
{
    static constexpr size_t MEASUREMENTS_PER_PACKET = ESP_NOW_MAX_PAYLOAD_SIZE / sizeof(LidarMeasurement);

    LidarMeasurement measurements[MEASUREMENTS_PER_PACKET];
};