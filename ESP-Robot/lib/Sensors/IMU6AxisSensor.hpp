#pragma once

#include <ISensor.hpp>
#include <I2CDevice.hpp>
#include <SensorManager.hpp>
#include <Arduino.h>
#include <Wire.h>

class IMU6AxisSensor : public ISensor, public I2CDevice
{
public:
    IMU6AxisSensor(TwoWire& wire, uint8_t address = 0x68)
        : I2CDevice(wire, address)
    {}

    void begin() override
    {
        setByte(PWR_MGMT_1_REG,   0b00000000); // enable MPU
        setByte(GYRO_CONFIG_REG,  0b00010000); // scale to 1000deg/s
        setByte(ACCEL_CONFIG_REG, 0b00010000); // scape to 8g
        setByte(CONFIG_REG,       0b00000010); // DLPF filter
    }

    void read() override
    {
        requestBytes(IMU_DATA_START, 14);

        auto& raw = SensorManager::instance().getRaw();
        auto& converted = SensorManager::instance().getConverted();

        for (int i = 0; i < 3; ++i)
        {
            raw.acceleration[i] = readShortMSBfirst();
            converted.acceleration_m_per_s2[i] = raw.acceleration[i] / 4095.f * 9.81f;

            raw.gyro[i] = readShortMSBfirst();
            converted.gyro_rad_per_s[i] = raw.gyro[i] / 65.5f * PI / 180.f;
        }

        raw.imu_temperature = readShortMSBfirst();
        converted.imu_temperature_C = raw.imu_temperature / 340.f + 36.53f;


        float ax = converted.acceleration_m_per_s2[0];
        float ay = converted.acceleration_m_per_s2[1];
        float az = converted.acceleration_m_per_s2[2];
        float gx = converted.gyro_rad_per_s[0];
        float gy = converted.gyro_rad_per_s[1];
        // float gz = converted.gyro_rad_per_s[2];

        const uint32_t now = millis();
        const float dt = (now - lastTime) / 1000.f;
        lastTime = now;

        const float apitch = atan2(-ax, sqrt(ay * ay + az * az));
        const float aroll = -atan2(ay, sqrt(ax * ax + az * az));

        converted.pitch_rad = COMP_FILTER_ALPHA * (converted.pitch_rad + gx * dt) + (1 - COMP_FILTER_ALPHA) * apitch;
        converted.roll_rad = COMP_FILTER_ALPHA * (converted.roll_rad + gy * dt) + (1 - COMP_FILTER_ALPHA) * aroll;
    }

private:
    static constexpr float COMP_FILTER_ALPHA = 0.85f;

    static constexpr byte PWR_MGMT_1_REG = 0x6B;
    static constexpr byte GYRO_CONFIG_REG = 0x1B;
    static constexpr byte ACCEL_CONFIG_REG = 0x1C;
    static constexpr byte CONFIG_REG = 0x1A;

    static constexpr byte IMU_DATA_START = 0x3B;

    uint32_t lastTime = 0;
};