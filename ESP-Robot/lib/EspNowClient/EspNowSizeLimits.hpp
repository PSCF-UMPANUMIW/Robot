#pragma once

#include <Payloads/PacketID.hpp>

static constexpr int MAX_PACKET_SIZE = 250;
static constexpr int MAX_PAYLOAD_SIZE = MAX_PACKET_SIZE - sizeof(PacketID);