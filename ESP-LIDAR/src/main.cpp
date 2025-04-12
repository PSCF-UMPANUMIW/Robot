#include <Arduino.h>

#include "pins.hpp"
#include "LidarReader.hpp"
#include "LidarPacket.hpp"

static LidarReader* lidarReader;

void printLidarPacket(LidarPacket const& p)
{
    Serial.print(p.angle);
    Serial.print(" ");
    Serial.print(p.speed);
    Serial.print(" ");
    for (int i = 0; i < 4; i++) 
    {
        Serial.print(p.distance[i]);
        Serial.print(" ");
    }
    Serial.println();
}

void setup()
{
    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1, LIDAR_TX, -1);

    lidarReader = new LidarReader(Serial1, printLidarPacket);
}

void loop()
{
    lidarReader->readData();
    delay(10);
}