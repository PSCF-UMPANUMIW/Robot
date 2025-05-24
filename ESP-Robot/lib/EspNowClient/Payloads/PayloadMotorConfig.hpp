#pragma once

#include <cstdint>
#include "../../WheelPlatform/SettingsLevel.hpp"

struct PayloadMotorConfig
{
    SettingsLevel speed, acceleration, current;
    bool enableSteppers;
};