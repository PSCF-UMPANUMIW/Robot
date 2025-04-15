#pragma once

#include <cstdint>

#include "ESP_NOW_Payloads/PacketType.hpp"

constexpr uint8_t BASE_STATION_MAC_ADDR[] = {0xC4, 0xD8, 0xD5, 0xBE, 0x92, 0xB4};
// constexpr uint8_t BASE_STATION_MAC_ADDR[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

constexpr size_t ESP_NOW_MAX_PACKET_SIZE = 250;
constexpr size_t ESP_NOW_MAX_PAYLOAD_SIZE = ESP_NOW_MAX_PACKET_SIZE - sizeof(PacketType);
