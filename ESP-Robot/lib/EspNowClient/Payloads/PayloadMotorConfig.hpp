#pragma once

#include <cstdint>

struct PayloadMotorConfig
{
    uint16_t rmsCurrent_mA;
    uint16_t microsteps;

    float maxSpeed;
    float maxAcceleration;
};