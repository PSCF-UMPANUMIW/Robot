#include <EspNowClient.hpp>

#include <WiFi.h>

EspNowClient& EspNowClient::instance()
{
    static EspNowClient instance;
    return instance;
}


void EspNowClient::begin()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    ESP_ERROR_CHECK(esp_now_init());

    esp_now_register_recv_cb(recvHandlerDispatcher);
}


void EspNowClient::addPeer(const byte* peerMacAddr)
{
    esp_now_peer_info_t peerInfo = {
        .channel = 0,
        .ifidx = WIFI_IF_STA,
        .encrypt = false,
    };
    memcpy(peerInfo.peer_addr, peerMacAddr, 6);

    ESP_ERROR_CHECK(esp_now_add_peer(&peerInfo));
}


void EspNowClient::recvHandlerDispatcher(const uint8_t* mac_addr, const uint8_t* data, int data_len)
{
    auto& client = instance();

    PacketID packetType = static_cast<PacketID>(data[0]);

    if (client.handlers.find(packetType) != client.handlers.end())
    {
        client.handlers[packetType](const_cast<uint8_t*>(data + sizeof(PacketID)));
    }
}