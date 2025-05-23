#pragma once

#include <SensorManager.hpp>
#include <ISensor.hpp>
#include <cstdint>

class GroundSensor : public ISensor
{
public:
    enum class Position
    {
        LEFT = 0,
        FRONT = 1,
        RIGHT = 2,
    };

    GroundSensor(Position position, uint8_t pin, uint16_t min, uint16_t max)
        : position(position), pin(pin), min(min), max(max)
    {}

    void begin() override
    {
        pinMode(pin, INPUT);
    }

    void read() override
    {
        const uint16_t raw = analogRead(pin);
        const float normalized = constrain(static_cast<float>(raw - min) / (max - min), 0.f, 1.f);

        auto& sm = SensorManager::instance();

        int posIdx = static_cast<int>(position);

        sm.getRaw().ground_sensor[posIdx] = raw;
        updateWithExpFilter(sm.getConverted().ground_sensor_normalized[posIdx], normalized, EXP_ALPHA);
    }

private:
    static constexpr float EXP_ALPHA = 0.15;

    Position position;
    uint8_t pin;
    uint16_t min, max;
};