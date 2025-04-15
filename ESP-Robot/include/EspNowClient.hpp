#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

#include "constants.hpp"
#include "ESP_NOW_Payloads/PacketType.hpp"
#include "ESP_NOW_Payloads/PayloadTraits.hpp"

class EspNowClient
{
public:
    EspNowClient()
    {
        WiFi.mode(WIFI_STA);
        WiFi.disconnect();

        if (esp_now_init() != ESP_OK)
        {
            Serial.println("Error initializing ESP-NOW.");
        }

        memcpy(peerInfo.peer_addr, BASE_STATION_MAC_ADDR, 6);
        peerInfo.channel = 0;
        peerInfo.encrypt = false;
        peerInfo.ifidx = WIFI_IF_STA;

        if (esp_now_add_peer(&peerInfo) != ESP_OK)
        {
            Serial.println("Error adding ESP-NOW peer.");
        }
    }

    template<typename PayloadType>
    void sendMessage(PayloadType payload)
    {
        constexpr PacketType ID = PayloadTraits<PayloadType>::packetType;

        static_assert(ID != PacketType::UNSPECIFIED, "Payload type not supported.");

        sendMessage<ID>(payload);
    }
    

private:
    template<PacketType ID, typename PayloadType>
    void sendMessage(PayloadType payload)
    {
        const size_t idSize      = sizeof(ID);
        const size_t payloadSize = sizeof(PayloadType);
        const size_t messageSize = idSize + payloadSize;

        if (messageSize > 250) {
            Serial.println("Message payload too big");
            return;
        }

        uint8_t buffer[messageSize];

        auto id = ID;
        memcpy(buffer, &id, idSize);
        memcpy(buffer + idSize, &payload, payloadSize);

        esp_err_t result = esp_now_send(BASE_STATION_MAC_ADDR, buffer, messageSize);

        if (result != ESP_OK)
        {
            Serial.println("Error sending ESP-NOW message.");
        }
    }

    esp_now_peer_info_t peerInfo;
};