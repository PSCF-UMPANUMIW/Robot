#pragma once

#include "PacketType.hpp"
#include "PayloadLidar.hpp"
#include "PayloadLog.hpp"
#include "PayloadSensors.hpp"

template<typename PayloadType>
struct PayloadTraits
{
    static constexpr PacketType packetType = PacketType::UNSPECIFIED;
};

template<>
struct PayloadTraits<PayloadLidar>
{
    static constexpr PacketType packetType = PacketType::LIDAR;
};

template<>
struct PayloadTraits<PayloadLog>
{
    static constexpr PacketType packetType = PacketType::LOG;
};

template<>
struct PayloadTraits<PayloadSensors>
{
    static constexpr PacketType packetType = PacketType::SENSORS;
};