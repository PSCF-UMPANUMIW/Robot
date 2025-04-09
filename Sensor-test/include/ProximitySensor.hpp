#pragma once

#include <Arduino.h>

class ProximitySensor
{
public:
    ProximitySensor(uint8_t pin, uint16_t min = 0, uint16_t max = 4095)
        : pin(pin), minValue(min), maxValue(max)
    {}

    void init()
    {
        pinMode(pin, INPUT);
    }

    float getValue()
    {
        int rawValue = analogRead(pin);
        float value = (rawValue - minValue) / static_cast<float>(maxValue - minValue); // Normalize to 0-1 range
        return constrain(value, 0.f, 1.f);
    }

private:
    uint8_t pin;
    uint16_t minValue;
    uint16_t maxValue;
};