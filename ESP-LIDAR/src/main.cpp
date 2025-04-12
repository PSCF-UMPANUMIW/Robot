#include <Arduino.h>

#include "pins.hpp"
#include "LidarReader.hpp"
#include "LidarPacket.hpp"

static LidarReader* lidarReader;

void setup()
{
    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1, LIDAR_TX, -1);

    lidarReader = new LidarReader(Serial1, [](LidarPacket const& lp) {
        Serial.print("Angle: ");
        Serial.print(lp.angle);
        Serial.print(", Speed: ");
        Serial.print(lp.speed);
        Serial.print(", Distances: ");
        for (int i = 0; i < 4; i++)
        {
            Serial.print(lp.distance[i]);
            if (i < 3) Serial.print(", ");
        }
        Serial.println();
    });
}

void loop()
{
    lidarReader->readData();
    delay(10);
}