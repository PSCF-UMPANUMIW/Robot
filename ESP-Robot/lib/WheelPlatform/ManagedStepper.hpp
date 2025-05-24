#pragma once

#include <AccelStepper.h>
#include <TMCStepper.h>

class ManagedStepper
{
public:
    ManagedStepper(AccelStepper& stepper, TMCStepper& driver, float stepsPerMeter)
        : stepper(stepper), driver(driver), stepsPerMeter(stepsPerMeter)
    {}

    void setMaxSpeed(float speed_mps)
    {
        stepper.setMaxSpeed(metersToSteps(speed_mps));
    }

    void setAcceleration(float accel_mps2)
    {
        stepper.setAcceleration(metersToSteps(accel_mps2));
    }

    void setCurrent(uint16_t current_mA)
    {
        driver.rms_current(current_mA);
    }

    void enable()
    {
        stepper.enableOutputs();
    }

    void disable()
    {
        stepper.disableOutputs();
    }

    void move(float meters)
    {
        stepper.move(metersToSteps(meters));
    }

    float getPosition()
    {
        return stepsToMeters(stepper.currentPosition());
    }

    void run()
    {
        stepper.run();
    }

private:
    float stepsToMeters(long steps) { return steps / stepsPerMeter; }
    long metersToSteps(float meters) { return static_cast<long>(meters * stepsPerMeter); };

    AccelStepper& stepper;
    TMCStepper& driver;

    float stepsPerMeter;
};