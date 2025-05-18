#include <Arduino.h>
#include "EspNowClient.hpp"

#include "ESP_NOW_Payloads/PayloadMoveCommand.hpp"

void setup()
{
    Serial.begin(115200);

    EspNowClient::init();
    EspNowClient::addPeer(ROBOT_MAC_ADDR);

    Serial.println("Base Station Setup Complete");
}

void sendMove(long l, long r)
{
    PayloadMoveCommand comm {
        .distanceL = l,
        .distanceR = r,
    };

    EspNowClient::sendMessage(comm);
}


static float maxSpeed = 400.0f;
static float maxAcceleration = 400.0f;

void sendMotorConf()
{
    PacketMotorConfig conf {
        .rmsCurrent_mA = 1200,
        .microsteps = 0,
        .maxSpeed = maxSpeed,
        .maxAcceleration = maxAcceleration,
    };

    EspNowClient::sendMessage(conf);
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
            case '2':
                sendMove(2000, 2000);
                break;
            case 'w':
                sendMove(200, 200);
                break;
            case 's':
                sendMove(-200, -200);
                break;
            case 'a':
                sendMove(-50, 50);
                break;
            case 'd':
                sendMove(50, -50);
                break;
            case 'i':
                maxSpeed += 50.0f;
                sendMotorConf();
                Serial.printf("Max Speed: %.2f\n", maxSpeed);
                break;
            case 'k':
                maxSpeed -= 50.0f;
                sendMotorConf();
                Serial.printf("Max Speed: %.2f\n", maxSpeed);
                break;
            case 'o':
                maxAcceleration += 50.0f;
                sendMotorConf();
                Serial.printf("Max Acceleration: %.2f\n", maxAcceleration);
                break;
            case 'l':
                maxAcceleration -= 50.0f;
                sendMotorConf();
                Serial.printf("Max Acceleration: %.2f\n", maxAcceleration);
                break;
            default:
                return;
        }

        delay(200);
    }
}