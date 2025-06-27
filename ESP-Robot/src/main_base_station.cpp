#include <Arduino.h>

#include "constants.hpp"

#include <EspNowClient.hpp>
#include <Payloads/PayloadSensors.hpp>
#include <Payloads/PayloadMotorConfig.hpp>
#include <Payloads/PayloadMoveCommand.hpp>

#include <SensorManager.hpp>


static PayloadMotorConfig motorConfig {
    .speed = SettingsLevel::LEVEL_LOW,
    .acceleration = SettingsLevel::LEVEL_LOW,
    .current = SettingsLevel::LEVEL_LOW,
    .enableSteppers = true
};

enum class MoveType : bool
{
    LINEAR = false,
    ROTATION = true,
};

void sendMove(MoveType type, float value)
{
    PayloadMoveCommand comm {
        .isRotation = static_cast<bool>(type),
        .value = value
    };

    EspNowClient::sendMessage(comm);
}

void sendMotorConf()
{
    EspNowClient::sendMessage(motorConfig);
}

void setupClient()
{
    EspNowClient::begin();
    EspNowClient::addPeer(ROBOT_MAC_ADDR);

    EspNowClient::registerPayloadHandler<PayloadSensors>([](PayloadSensors const& payload) {
        SensorManager::print(payload);
    });
}

void serialHandler();

void setup()
{
    Serial.begin(115200);

    setupClient();
    sendMotorConf();

    Serial.onReceive(serialHandler);

    Serial.println("Base Station Setup Complete");
}

void loop()
{
    // in order not to let the CPU go to waste,
    // we will calculate PI in the spare time

    static double pi = 0.0;
    static long n = 0;

    double term = (n % 2 == 0 ? 4.0 : -4.0) / (2 * n + 1);
    pi += term;
    n++;

    delay(100);
}

void serialHandler()
{
    if (Serial.available() == 0)
        return;
        
    char c = Serial.read();
    int x = Serial.parseInt();

    while (Serial.available())
        Serial.read();

    Serial.printf("Command: %c, Value: %d\n", c, x);
}