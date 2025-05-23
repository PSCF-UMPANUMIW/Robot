#pragma once

#include <Arduino.h>
#include <ISensor.hpp>
#include <SensorManager.hpp>

class ProximitySensor : public ISensor
{
public:
    ProximitySensor(uint8_t pinTrig, uint8_t pinEcho, float maxDistance)
        : pinTrig(pinTrig), pinEcho(pinEcho), maxDistance(maxDistance)
    {}

    void begin() override
    {
        pinMode(pinTrig, OUTPUT);
        pinMode(pinEcho, INPUT);
        digitalWrite(pinTrig, LOW);
    }

    void read() override
    {
        digitalWrite(pinTrig, LOW);
        delayMicroseconds(2);
        digitalWrite(pinTrig, HIGH);
        delayMicroseconds(10);
        digitalWrite(pinTrig, LOW);

        const long duration_us = pulseIn(pinEcho, HIGH, 5000);
        const float distance_m = (duration_us * 1e-6 / 2.f) * SPEED_OF_SOUND_MPS;

        auto& sm = SensorManager::instance();
        sm.getRaw().hc_pulse_duration = duration_us;

        updateWithExpFilter(sm.getConverted().hc_distance_m, distance_m, EXP_ALPHA);
    }


private:
    static constexpr float SPEED_OF_SOUND_MPS = 343.0f;
    static constexpr float EXP_ALPHA = 0.15;

    uint8_t pinTrig, pinEcho;
    float maxDistance;
};