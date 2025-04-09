#pragma once

#include <Arduino.h>

class HC_SR04
{
public:
    HC_SR04(uint8_t trigPin, uint8_t echoPin)
        : trigPin(trigPin), echoPin(echoPin)
    {}

    void begin()
    {
        pinMode(trigPin, OUTPUT);
        pinMode(echoPin, INPUT);
        digitalWrite(trigPin, LOW);
    }

    float getDistance()
    {
        trigger();

        long duration = pulseIn(echoPin, HIGH, TIMEOUT);
        if (duration == 0)
            return -1.f; // Timeout

        float distance = (duration * SOUND_SPEED) / 2.f; // Convert to mm
        return distance;
    }

private:
    static constexpr uint32_t TIMEOUT = 30000; // 30ms timeout for echo response
    static constexpr float SOUND_SPEED = 0.343; // mm/us

    void trigger()
    {
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
    }

    uint8_t trigPin;
    uint8_t echoPin;
};