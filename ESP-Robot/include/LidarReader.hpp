#pragma once

#include <Arduino.h>

#include "LidarPacket.hpp"

class LidarReader
{
public:
    using DataReadyCallback = void (*)(LidarPacket const&);

    LidarReader(HardwareSerial& serial, DataReadyCallback callback)
        : uart(serial), callback(callback)
    {}

    void readData()
    {
        while (uart.available())
        {
            const byte b = uart.read();

            if (b == HEAD_BYTE)
                packetIndex = 0;

            packet[packetIndex++] = b;

            if (packetIndex >= PACKET_SIZE)
            {
                LidarPacket lp = decodePacket();
                if (callback) callback(lp);
                packetIndex = PACKET_SIZE-1;
            }
        }
    }

    LidarPacket decodePacket() const
    {
        LidarPacket lp;

        lp.angle = (packet[1] - 0xA0) * 4;
        lp.speed = ((uint16_t)packet[3] << 8) | packet[2];

        for (int i = 0; i < DISTANCES_PER_PACKET; i++)
        {
            int pi = 4 + 4 * i;
            lp.distance[i] = (((uint16_t)packet[pi + i] & 0x3D) << 8) | packet[pi];
        }

        return lp;
    }

private:
    static const int PACKET_SIZE = 22;
    static const int DISTANCES_PER_PACKET = 4;
    static const byte HEAD_BYTE = 0xFA;

    HardwareSerial& uart;
    DataReadyCallback callback;

    byte packet[PACKET_SIZE];
    int  packetIndex = 0;
};