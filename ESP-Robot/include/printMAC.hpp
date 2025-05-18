#include <Arduino.h>

void printMAC()
{
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);

    Serial.print("MAC Address: ");
    for (int i = 0; i < 6; ++i)
    {
        Serial.print(mac[i], HEX);
        if (i < 5)
            Serial.print(":");
    }
    Serial.println();
}