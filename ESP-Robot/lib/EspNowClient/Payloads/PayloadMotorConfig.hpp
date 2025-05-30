#pragma once

#include <cstdint>
#include <MotorSettingsLevel.hpp>

struct PayloadMotorConfig
{
    SettingsLevel speed, acceleration, current;
    bool enableSteppers;
};