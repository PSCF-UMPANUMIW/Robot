#include <WheelPlatform.hpp>
#include <StepperConstants.hpp>

WheelPlatform::WheelPlatform(ManagedStepper& left, ManagedStepper& right, float wheelbase)
    : stepperL(left), stepperR(right), wheelbase(wheelbase)
{}

void WheelPlatform::begin()
{
    setAcceleration(SettingsLevel::LEVEL_LOW);
    setMaxSpeed(SettingsLevel::LEVEL_LOW);
    setDriverCurrent(SettingsLevel::LEVEL_LOW);
    enableSteppers();
}

void WheelPlatform::setAcceleration(SettingsLevel level)
{
    switch (level)
    {
    case SettingsLevel::LEVEL_LOW:
        stepperL.setAcceleration(ACCELERATION_LOW);
        stepperR.setAcceleration(ACCELERATION_LOW);
        break;
    case SettingsLevel::LEVEL_MEDIUM:
        stepperL.setAcceleration(ACCELERATION_MEDIUM);
        stepperR.setAcceleration(ACCELERATION_MEDIUM);
        break;
    case SettingsLevel::LEVEL_HIGH:
        stepperL.setAcceleration(ACCELERATION_HIGH);
        stepperR.setAcceleration(ACCELERATION_HIGH);
        break;

    default:
        Serial.println("[WARN] Invalid acceleration level");
        break;
    }
}

void WheelPlatform::setMaxSpeed(SettingsLevel level)
{
    switch (level)
    {
    case SettingsLevel::LEVEL_LOW:
        stepperL.setMaxSpeed(SPEED_LOW);
        stepperR.setMaxSpeed(SPEED_LOW);
        break;
    case SettingsLevel::LEVEL_MEDIUM:
        stepperL.setMaxSpeed(SPEED_MEDIUM);
        stepperR.setMaxSpeed(SPEED_MEDIUM);
        break;
    case SettingsLevel::LEVEL_HIGH:
        stepperL.setMaxSpeed(SPEED_HIGH);
        stepperR.setMaxSpeed(SPEED_HIGH);
        break;

    default:
        Serial.println("[WARN] Invalid speed level");
        break;
    }
}

void WheelPlatform::setDriverCurrent(SettingsLevel level)
{
    switch (level)
    {
    case SettingsLevel::LEVEL_LOW:
        stepperL.setCurrent(CURRENT_LOW);
        stepperR.setCurrent(CURRENT_LOW);
        break;
    case SettingsLevel::LEVEL_MEDIUM:
        stepperL.setCurrent(CURRENT_MEDIUM);
        stepperR.setCurrent(CURRENT_MEDIUM);
        break;
    case SettingsLevel::LEVEL_HIGH:
        stepperL.setCurrent(CURRENT_HIGH);
        stepperR.setCurrent(CURRENT_HIGH);
        break;

    default:
        Serial.println("[WARN] Invalid current level");
        break;
    }
}

void WheelPlatform::enableSteppers()
{
    stepperL.enable();
    stepperR.enable();
}

void WheelPlatform::disableSteppers()
{
    stepperL.disable();
    stepperR.disable();
}

void WheelPlatform::move(float distance)
{
    stepperL.move(distance);
    stepperR.move(distance);
}

void WheelPlatform::turn(float angle)
{
    const float wheelDistance = wheelbase * angle / 2;

    Serial.printf("Turning %f radians by moving each wheel %f meters", angle, wheelDistance);

    stepperL.move(wheelDistance);
    stepperR.move(-wheelDistance);
}

void WheelPlatform::run()
{
    float posL = stepperL.getPosition();
    float posR = stepperR.getPosition();

    stepperL.run();
    stepperR.run();

    float deltaL = stepperL.getPosition() - posL;
    float deltaR = stepperR.getPosition() - posR;

    wheelPosL.x += deltaL * cos(theta);
    wheelPosL.y += deltaL * sin(theta);

    wheelPosR.x += deltaR * cos(theta);
    wheelPosR.y += deltaR * sin(theta);

    theta = atan2(wheelPosR.y - wheelPosL.y, wheelPosR.x - wheelPosL.x);
}

WheelPlatform::OdometryPosition WheelPlatform::getOdometryPosition() const
{
    vec2f center = (wheelPosL + wheelPosR) / 2.f;
    return {center.x, center.y, theta};
}

void WheelPlatform::setOdometryPosition(const OdometryPosition& position)
{
    wheelPosL.x = position.x - (wheelbase / 2) * cos(position.theta);
    wheelPosL.y = position.y - (wheelbase / 2) * sin(position.theta);

    wheelPosR.x = position.x + (wheelbase / 2) * cos(position.theta);
    wheelPosR.y = position.y + (wheelbase / 2) * sin(position.theta);

    theta = position.theta;
}