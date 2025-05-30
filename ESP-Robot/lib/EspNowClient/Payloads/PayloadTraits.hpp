#pragma once

#include <Payloads/PacketID.hpp>

struct PayloadLidar;
struct PayloadLog;
struct PayloadSensors;
struct PayloadMoveCommand;
struct PayloadMotorConfig;


template<typename PayloadType>
struct PayloadTraits
{
    static constexpr PacketID packetType = PacketID::UNSPECIFIED;
};


template<>
struct PayloadTraits<PayloadLidar>
{
    static constexpr PacketID packetType = PacketID::LIDAR;
};

template<>
struct PayloadTraits<PayloadLog>
{
    static constexpr PacketID packetType = PacketID::LOG;
};

template<>
struct PayloadTraits<PayloadSensors>
{
    static constexpr PacketID packetType = PacketID::SENSORS;
};

template<>
struct PayloadTraits<PayloadMoveCommand>
{
    static constexpr PacketID packetType = PacketID::MOVE_COMMAND;
};

template<>
struct PayloadTraits<PayloadMotorConfig>
{
    static constexpr PacketID packetType = PacketID::MOTOR_CONFIG;
};