// #pragma once

// #include <Arduino.h>
// #include <WiFi.h>
// #include <esp_wifi.h>
// #include <esp_now.h>

// #include "LidarPacket.hpp"
// #include "constants.hpp"

// class EspNowClient
// {
// public:
//     EspNowClient()
//     {
//         WiFi.mode(WIFI_STA);
//         WiFi.disconnect();

//         if (esp_now_init() != ESP_OK)
//         {
//             Serial.println("Error initializing ESP-NOW.");
//         }

//         memcpy(peerInfo.peer_addr, BASE_STATION_MAC_ADDR, 6);
//         peerInfo.channel = 0;
//         peerInfo.encrypt = false;

//         if (esp_now_add_peer(&peerInfo) != ESP_OK)
//         {
//             Serial.println("Error adding ESP-NOW peer.");
//         }
//     }

//     void addLidarPacket(LidarPacket const& lp)
//     {
//         lidarPackets[lidarPacketsIndex++] = lp;

//         if (lidarPacketsIndex >= LIDAR_PACKETS_PER_ESP_NOW_MESSAGE)
//         {
//             esp_err_t result = esp_now_send(BASE_STATION_MAC_ADDR, (uint8_t*)lidarPackets, sizeof(lidarPackets));

//             if (result != ESP_OK)
//             {
//                 Serial.println("Error sending ESP-NOW message.");
//             }

//             lidarPacketsIndex = 0;
//         }
//     }

// private:
//     static constexpr int MAX_MSG_LENGTH = 250;
//     static constexpr int LIDAR_PACKETS_PER_ESP_NOW_MESSAGE = MAX_MSG_LENGTH / sizeof(LidarPacket);

//     esp_now_peer_info_t peerInfo;

//     LidarPacket lidarPackets[LIDAR_PACKETS_PER_ESP_NOW_MESSAGE];
//     int lidarPacketsIndex = 0;
// };