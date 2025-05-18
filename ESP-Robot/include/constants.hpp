#pragma once

#include <cstdint>

#include "ESP_NOW_Payloads/PacketID.hpp"

constexpr uint8_t BASE_STATION_MAC_ADDR[] = {0xC4, 0xD8, 0xD5, 0xBE, 0x92, 0xB4};
constexpr uint8_t ROBOT_MAC_ADDR[] = {0x34, 0x5F, 0x45, 0xAA, 0x20, 0x18};
constexpr uint8_t BROADCAST_MAC_ADDR[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

constexpr size_t ESP_NOW_MAX_PACKET_SIZE = 250;
constexpr size_t ESP_NOW_MAX_PAYLOAD_SIZE = ESP_NOW_MAX_PACKET_SIZE - sizeof(PacketID);
