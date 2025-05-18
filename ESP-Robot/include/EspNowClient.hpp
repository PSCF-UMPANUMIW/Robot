#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

#include <unordered_map>

#include "constants.hpp"
#include "ESP_NOW_Payloads/PacketID.hpp"
#include "ESP_NOW_Payloads/PayloadTraits.hpp"

class EspNowClient
{
public:
    static void init()
    {
        WiFi.mode(WIFI_STA);
        WiFi.disconnect();

        ESP_ERROR_CHECK(esp_now_init());

        esp_now_register_recv_cb(recvHandlerDelegate);
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
        constexpr PacketID ID = PayloadTraits<PayloadType>::packetType;

        static_assert(ID != PacketID::UNSPECIFIED, "Payload type not supported.");
        
        constexpr size_t ID_SIZE      = sizeof(ID);
        constexpr size_t PAYLOAD_SIZE = sizeof(payload);
        constexpr size_t MESSAGE_SIZE = ID_SIZE + PAYLOAD_SIZE;

        static_assert(MESSAGE_SIZE <= ESP_NOW_MAX_PACKET_SIZE, "Payload size exceeds maximum limit.");

        uint8_t buffer[MESSAGE_SIZE];
        memcpy(buffer,           &ID,      ID_SIZE);
        memcpy(buffer + ID_SIZE, &payload, PAYLOAD_SIZE);

        ESP_ERROR_CHECK(esp_now_send(NULL, buffer, MESSAGE_SIZE)); // NULL = send to all registered peers
    }

    template<typename TPacket>
    static void registerPayloadHandler(std::function<void(TPacket const& packet)> handler)
    {
        auto newHandler = [handler](const void* rawData) {
            handler(*reinterpret_cast<const TPacket*>(rawData));
        };
        
        PacketID packetType = PayloadTraits<TPacket>::packetType;
        s_handlers[packetType] = newHandler;
    }

private:
    using HandlerBase = std::function<void(const void*)>;

    static void recvHandlerDelegate(const uint8_t* mac_addr, const uint8_t* data, int data_len)
    {
        PacketID packetType = static_cast<PacketID>(data[0]);

        if (s_handlers.find(packetType) != s_handlers.end())
        {
            s_handlers[packetType](const_cast<uint8_t*>(data + sizeof(PacketID)));
        }
    }

    static std::unordered_map<PacketID, HandlerBase> s_handlers;
};