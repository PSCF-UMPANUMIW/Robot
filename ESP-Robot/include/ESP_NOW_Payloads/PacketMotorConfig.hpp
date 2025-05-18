#pragma once

#include <cstdint>

struct PacketMotorConfig
{
    uint16_t rmsCurrent_mA;
    uint16_t microsteps;

    float maxSpeed;
    float maxAcceleration;
};