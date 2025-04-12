#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

struct LidarPacket
{
    uint16_t angle;
    uint16_t speed;
    uint16_t distance[4];
};

void onDataRecv(const uint8_t* mac, const uint8_t* data, int len)
{
    LidarPacket* p = (LidarPacket*)data;

    Serial.print(p->angle);
    Serial.print(" ");
    Serial.print(p->speed);
    Serial.print(" ");
    for (int i = 0; i < 4; i++) 
    {
        Serial.print(p->distance[i]);
        Serial.print(" ");
    }
    Serial.println();

    // for (int i = 0; i < len; i++) {
    //     Serial.printf("%02X ", data[i]);
    // }
    // Serial.println("");
}

void setup()
{
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW.");
    }

    esp_now_register_recv_cb(onDataRecv);
}

void loop()
{
    delay(1000);
}