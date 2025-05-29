#include <Arduino.h>

#include "constants.hpp"
#include "printmac.hpp"

#include <EspNowClient.hpp>
#include <SensorManager.hpp>

static constexpr bool ROTATION = true;
static constexpr bool FORWARD  = false;

static PayloadMotorConfig motorConfig {
    .speed = SettingsLevel::LEVEL_LOW,
    .acceleration = SettingsLevel::LEVEL_LOW,
    .current = SettingsLevel::LEVEL_LOW,
    .enableSteppers = true
};

void sendMove(bool isRotation, float value)
{
    PayloadMoveCommand comm {
        .isRotation = isRotation,
        .value = value
    };

    EspNowClient::instance().sendMessage(comm);
}

void sendMotorConf()
{
    EspNowClient::instance().sendMessage(motorConfig);
}

void setupClient()
{
    auto& client = EspNowClient::instance();

    client.begin();
    client.addPeer(ROBOT_MAC_ADDR);

    client.registerPayloadHandler<PayloadSensors>([](PayloadSensors const& payload) {
        SensorManager::instance().print(payload);
    });
}


void setup()
{
    Serial.begin(115200);

    setupClient();
    sendMotorConf();
    printMacAddress();

    Serial.println("Base Station Setup Complete");
}

void loop()
{
    if (Serial.available())
    {
        const char c = Serial.read();

        while (Serial.available())
            Serial.read();

        switch (c)
        {
            case 'e':
                sendMove(FORWARD, 2.f);
                break;
            case 'w':
                sendMove(FORWARD, 0.2f);
                break;
            case 's':
                sendMove(FORWARD, -0.2f);
                break;
            case 'a':
                sendMove(ROTATION, -PI/8.f);
                break;
            case 'd':
                sendMove(ROTATION, PI/8.f);
                break;
            
            case '1':
                motorConfig.speed = SettingsLevel::LEVEL_LOW;
                sendMotorConf();
                Serial.println("Speed set to LOW");
                break;
            case '2':
                motorConfig.speed = SettingsLevel::LEVEL_MEDIUM;
                sendMotorConf();
                Serial.println("Speed set to MEDIUM");
                break;
            case '3':
                motorConfig.speed = SettingsLevel::LEVEL_HIGH;
                sendMotorConf();
                Serial.println("Speed set to HIGH");
                break;

            case '4':
                motorConfig.acceleration = SettingsLevel::LEVEL_LOW;
                sendMotorConf();
                Serial.println("Acceleration set to LOW");
                break;
            case '5':
                motorConfig.acceleration = SettingsLevel::LEVEL_MEDIUM;
                sendMotorConf();
                Serial.println("Acceleration set to MEDIUM");
                break;
            case '6':
                motorConfig.acceleration = SettingsLevel::LEVEL_HIGH;
                sendMotorConf();
                Serial.println("Acceleration set to HIGH");
                break;

            case '7':
                motorConfig.current = SettingsLevel::LEVEL_LOW;
                sendMotorConf();
                Serial.println("Current set to LOW");
                break;
            case '8':
                motorConfig.current = SettingsLevel::LEVEL_MEDIUM;
                sendMotorConf();
                Serial.println("Current set to MEDIUM");
                break;
            case '9':
                motorConfig.current = SettingsLevel::LEVEL_HIGH;
                sendMotorConf();
                Serial.println("Current set to HIGH");
                break;

            default:
                return;
        }

        delay(200);
    }
}