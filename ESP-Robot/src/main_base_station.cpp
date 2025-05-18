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

void loop()
{
    if (Serial.available())
    {
        const char c = Serial.read();

        while (Serial.available())
            Serial.read();

        PayloadMoveCommand comm;

        switch (c)
        {
            case 'w':
                comm.distanceL = 100;
                comm.distanceR = 100;
                break;
            case 's':
                comm.distanceL = -100;
                comm.distanceR = -100;
                break;
            case 'a':
                comm.distanceL = -100;
                comm.distanceR = 100;
                break;
            case 'd':
                comm.distanceL = 100;
                comm.distanceR = -100;
                break;
            default:
                return;
        }

        EspNowClient::sendMessage(comm);

        delay(200);
    }
}