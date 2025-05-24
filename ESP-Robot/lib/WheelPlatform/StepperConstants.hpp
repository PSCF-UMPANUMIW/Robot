#pragma once

#include <cstdint>

// speed limit in m/s
constexpr float SPEED_LOW    = 0.2f;
constexpr float SPEED_MEDIUM = 0.2f;
constexpr float SPEED_HIGH   = 1.0f;

// acceleration limit in m/s^2
constexpr float ACCELERATION_LOW    = 0.2f;
constexpr float ACCELERATION_MEDIUM = 0.4f;
constexpr float ACCELERATION_HIGH   = 0.8f;

// current limits in mA
constexpr uint16_t CURRENT_LOW    = 400; // set 400 measures 718
constexpr uint16_t CURRENT_MEDIUM = 1000;
constexpr uint16_t CURRENT_HIGH   = 1400;