#pragma once

#include <ManagedStepper.hpp>
#include <MotorSettingsLevel.hpp>

class WheelPlatform
{
public:
    WheelPlatform(ManagedStepper& left, ManagedStepper& right, float wheelbase);
    ~WheelPlatform() = default;

    struct OdometryPosition
    {
        float x;
        float y;
        float theta;
    };

    void begin();
    void setMaxSpeed(SettingsLevel speed);
    void setAcceleration(SettingsLevel acceleration);
    void setDriverCurrent(SettingsLevel current);
    void enableSteppers();
    void disableSteppers();

    void move(float distance);
    void turn(float angle);
    void run();

    OdometryPosition getOdometryPosition() const;
    void setOdometryPosition(const OdometryPosition& position);

private:
    struct vec2f
    {
        float x, y;
        vec2f operator+(const vec2f& other) const { return {x + other.x, y + other.y}; }
        vec2f operator-(const vec2f& other) const { return {x - other.x, y - other.y}; }
        vec2f operator*(float scalar) const { return {x * scalar, y * scalar}; }
        vec2f operator/(float scalar) const { return {x / scalar, y / scalar}; }
    };

    vec2f wheelPosL, wheelPosR;
    float theta;

    ManagedStepper& stepperL;
    ManagedStepper& stepperR;

    float wheelbase;
};