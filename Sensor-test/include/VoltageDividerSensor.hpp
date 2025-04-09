#pragma once

#include <Arduino.h>

class VoltageDividerSensor
{
public:
    VoltageDividerSensor(uint8_t pin, float r_vcc, float r_gnd)
        : pin(pin), multiplier((r_vcc + r_gnd) / r_gnd)
    {}

    VoltageDividerSensor(uint8_t pin, float multiplier)
        : pin(pin), multiplier(multiplier)
    {}

    void init()
    {
        pinMode(pin, INPUT);
    }

    float getVoltage()
    {
        int rawValue = analogRead(pin);
        float voltage = (rawValue / 4095.f) * 3.3f; // Convert to voltage (0-3.3V)
        return voltage * multiplier; // Apply voltage divider formula
    }

private:
    uint8_t pin;
    float multiplier = 1.f;
};