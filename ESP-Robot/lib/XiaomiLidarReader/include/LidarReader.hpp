#pragma once

#include <Arduino.h>
#include <functional>

#include "LidarMeasurement.hpp"
#include "Buffer.hpp"

class LidarReader
{
public:
    using DataReadyCallback = std::function<void(LidarMeasurement const&)>;

    LidarReader(HardwareSerial& serial, DataReadyCallback const& callback);
    void readData();
    
private:
    static const size_t FRAME_SIZE = 22;
    static const int DISTANCES_PER_PACKET = 4;
    static const byte HEAD_BYTE = 0xFA;
    
    LidarMeasurement decodeMeasurement() const;

    HardwareSerial& uart;
    DataReadyCallback callback;
    Buffer<byte, FRAME_SIZE> buffer;
};