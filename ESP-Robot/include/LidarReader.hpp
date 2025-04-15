#pragma once

#include <Arduino.h>
#include <functional>

#include "LidarMeasurement.hpp"
#include "Buffer.hpp"

class LidarReader
{
public:
    using DataReadyCallback = std::function<void(LidarMeasurement const&)>;

    LidarReader(HardwareSerial& serial, DataReadyCallback callback)
        : uart(serial), callback(callback)
    {}

    void readData()
    {
        while (uart.available())
        {
            const byte b = uart.read();

            if (b == HEAD_BYTE)
            {
                buffer.clear();
            }

            buffer.push(b);

            if (buffer.isFull())
            {
                LidarMeasurement lp = decodeMeasurement();
                if (callback) callback(lp);
            }
        }
    }

    LidarMeasurement decodeMeasurement() const
    {
        LidarMeasurement lp;

        lp.angle = (buffer[1] - 0xA0) * 4;
        lp.speed = ((uint16_t)buffer[3] << 8) | buffer[2];

        for (int i = 0; i < DISTANCES_PER_PACKET; i++)
        {
            int pi = 4 + 4 * i;
            lp.distance[i] = (((uint16_t)buffer[pi + i] & 0x3D) << 8) | buffer[pi];
        }

        return lp;
    }

private:
    static const int PACKET_SIZE = 22;
    static const int DISTANCES_PER_PACKET = 4;
    static const byte HEAD_BYTE = 0xFA;

    HardwareSerial& uart;
    DataReadyCallback callback;

    Buffer<byte, PACKET_SIZE> buffer;
};