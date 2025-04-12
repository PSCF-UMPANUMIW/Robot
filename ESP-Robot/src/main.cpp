#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

#include "pins.hpp"
#include "constants.hpp"
#include "LidarReader.hpp"
#include "LidarPacket.hpp"
#include "EspNowClient.hpp"

static LidarReader* lidarReader;
static EspNowClient* espNowClient;

void setup()
{
    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1, LIDAR_TX, -1);

    espNowClient = new EspNowClient();

    lidarReader = new LidarReader(Serial1, [](LidarPacket const& lp) {
        esp_err_t res = esp_now_send(BASE_STATION_MAC_ADDR, (uint8_t*)&lp, sizeof(lp));
        if (res != ESP_OK)
            Serial.printf("Error sending ESP-NOW message: %i = %s\n", res, esp_err_to_name(res));
    });
}

void loop()
{
    lidarReader->readData();
    delay(10);
}