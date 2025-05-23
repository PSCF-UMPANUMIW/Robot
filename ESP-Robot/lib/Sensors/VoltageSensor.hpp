#pragma once

#include <Arduino.h>
#include <ISensor.hpp>
#include <SensorManager.hpp>

class VoltageSensor : public ISensor
{
public:
    VoltageSensor(uint8_t pin, float fitA, float fitB, float fitC)
        : pin(pin), a(fitA), b(fitB), c(fitC)
    {}

    void begin() override
    {
        pinMode(pin, INPUT);
    }

    void read() override
    {
        const uint16_t counts = analogRead(pin);
        const float voltage = a * counts * counts + b * counts + c;

        auto& sm = SensorManager::instance();
        sm.getRaw().voltage_sensor = counts;

        auto& smVoltage = sm.getConverted().voltage_sensor_V;
        if (smVoltage == 0.f)
            smVoltage = voltage;
        else
            updateWithExpFilter(smVoltage, voltage, EXP_ALPHA);
    }

private:
    static constexpr float EXP_ALPHA = 0.02;
    
    uint8_t pin;
    float a, b, c;
};