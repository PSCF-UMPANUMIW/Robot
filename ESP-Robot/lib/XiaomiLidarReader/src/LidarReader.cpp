#include "LidarReader.hpp"
#include "LidarMeasurement.hpp"

LidarReader::LidarReader(HardwareSerial& serial, DataReadyCallback const& callback)
    : uart(serial), callback(callback)
{
    buffer.clear();
}

void LidarReader::readData()
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

LidarMeasurement LidarReader::decodeMeasurement() const
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