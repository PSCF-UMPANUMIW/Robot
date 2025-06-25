#pragma once

#include <Arduino.h>
#include <esp_now.h>

#include <unordered_map>

#include <Payloads/PayloadTraits.hpp>
#include <EspNowSizeLimits.hpp>

class EspNowClient
{
public:
    static EspNowClient& instance();

    static void begin();

    static void addPeer(const byte* peerMacAddr);

    template<typename PayloadType>
    static void sendMessage(PayloadType const& payload)
    {
        constexpr PacketID ID = PayloadTraits<PayloadType>::packetType;

        static_assert(ID != PacketID::UNSPECIFIED, "Payload type not supported.");
        
        constexpr size_t ID_SIZE      = sizeof(ID);
        constexpr size_t PAYLOAD_SIZE = sizeof(payload);
        constexpr size_t MESSAGE_SIZE = ID_SIZE + PAYLOAD_SIZE;

        static_assert(MESSAGE_SIZE <= MAX_PACKET_SIZE, "Payload size exceeds maximum limit.");

        uint8_t buffer[MESSAGE_SIZE];
        memcpy(buffer,           &ID,      ID_SIZE);
        memcpy(buffer + ID_SIZE, &payload, PAYLOAD_SIZE);

        ESP_ERROR_CHECK(esp_now_send(NULL, buffer, MESSAGE_SIZE)); // NULL = send to all registered peers
    }

    template<typename TPacket>
    static void registerPayloadHandler(std::function<void(TPacket const& packet)> handler)
    {
        auto typelessHandler = [handler](const void* rawData) {
            handler(*reinterpret_cast<const TPacket*>(rawData));
        };
        
        PacketID packetType = PayloadTraits<TPacket>::packetType;
        instance().handlers[packetType] = typelessHandler;
    }

private:
    EspNowClient() = default;
    ~EspNowClient() = default;
    
    EspNowClient(const EspNowClient&) = delete;
    EspNowClient(EspNowClient&&) = delete;
    EspNowClient& operator=(const EspNowClient&) = delete;
    EspNowClient& operator=(EspNowClient&&) = delete;

    static void recvHandlerDispatcher(const uint8_t* mac_addr, const uint8_t* data, int data_len);

    std::unordered_map<PacketID, std::function<void(const void*)>> handlers;
};