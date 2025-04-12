#pragma once

#include <Arduino.h>

struct LidarMeasurement
{
    uint16_t angle;
    uint16_t speed;
    uint16_t distance[4];
};