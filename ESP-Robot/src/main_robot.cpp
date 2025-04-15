#include <Arduino.h>

#include "pins.hpp"
#include "constants.hpp"

#include <LidarReader.hpp>

#include "EspNowClient.hpp"
#include "ESP_NOW_Payloads/PayloadLidar.hpp"
#include "LidarMeasurementBuffer.hpp"

static LidarReader* lidarReader;

void onLidarMeasurement(LidarMeasurement const& m)
{
    static LidarMeasurementBuffer buffer;
    buffer.add(m);
}

void setup()
{
    Serial.begin(115200);
    
    EspNowClient::init();
    EspNowClient::addPeer(BASE_STATION_MAC_ADDR);
    
    Serial1.begin(115200, SERIAL_8N1, LIDAR_TX, -1);
    lidarReader = new LidarReader(Serial1, onLidarMeasurement);

    Serial.println("Robot Setup Complete");
}

void loop()
{
    lidarReader->readData();
    delay(10);
}