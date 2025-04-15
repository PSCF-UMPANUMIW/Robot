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
    static void init()
    {
        WiFi.mode(WIFI_STA);
        WiFi.disconnect();

        if (esp_now_init() != ESP_OK)
        {
            Serial.println("Error initializing ESP-NOW.");
        }
    }

    static void addPeer(const byte* peerMacAddr)
    {
        esp_now_peer_info_t peerInfo = {
            .channel = 0,
            .ifidx = WIFI_IF_STA,
            .encrypt = false,
        };
        memcpy(peerInfo.peer_addr, peerMacAddr, 6);

        ESP_ERROR_CHECK(esp_now_add_peer(&peerInfo));
    }

    template<typename PayloadType>
    static void sendMessage(PayloadType const& payload)
    {
        constexpr PacketType ID = PayloadTraits<PayloadType>::packetType;

        static_assert(ID != PacketType::UNSPECIFIED, "Payload type not supported.");
        
        constexpr size_t ID_SIZE      = sizeof(ID);
        constexpr size_t PAYLOAD_SIZE = sizeof(payload);
        constexpr size_t MESSAGE_SIZE = ID_SIZE + PAYLOAD_SIZE;

        static_assert(MESSAGE_SIZE < ESP_NOW_MAX_PACKET_SIZE, "Payload size exceeds maximum limit.");

        uint8_t buffer[MESSAGE_SIZE];
        memcpy(buffer,           &ID,      ID_SIZE);
        memcpy(buffer + ID_SIZE, &payload, PAYLOAD_SIZE);

        ESP_ERROR_CHECK(esp_now_send(NULL, buffer, MESSAGE_SIZE)); // NULL = send to all registered peers
    }
};