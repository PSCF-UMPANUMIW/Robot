#pragma once

#include <Arduino.h>
#include <functional>

#include <LidarMeasurement.hpp>
#include <Buffer.hpp>

class LidarReader
{
public:
    using DataReadyCallback = std::function<void(LidarMeasurement const&)>;

    LidarReader(HardwareSerial& serial, uint8_t rxPin, uint8_t enPin, uint8_t motorEnPin);
    void begin();
    void setDataReadyCallback(DataReadyCallback const& callback) { this->callback = callback; }

    void enableLaser() { digitalWrite(enPin, HIGH); }
    void disableLaser() { digitalWrite(enPin, LOW); }

    enum class MotorSpeed : uint8_t { OFF, LOW_SPEED, HIGH_SPEED, TOO_FAST };

    void setMotorSpeed(MotorSpeed speed);
    void readData();

    ~LidarReader() = default;
    
private:
    static constexpr int FRAME_SIZE = 22;
    static constexpr int DISTANCES_PER_PACKET = 4;
    static constexpr byte HEAD_BYTE = 0xFA;

    static constexpr int LEDC_CHANNEL = 0;
    
    LidarMeasurement decodeMeasurement() const;

    HardwareSerial& uart;
    uint8_t rxPin, enPin, motorEnPin;

    DataReadyCallback callback;

    Buffer<byte, FRAME_SIZE> buffer;
};