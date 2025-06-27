#pragma once

#include <cstdint>
#include <LidarReader.hpp>

struct PayloadLidarConfig
{
    LidarReader::MotorSpeed motorSpeed;
    bool enableLaser;
};