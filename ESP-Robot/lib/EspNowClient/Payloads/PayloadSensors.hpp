#pragma once

#include <cstdint>

struct PayloadSensors
{
    struct ReadingsRaw
    {
        uint16_t ground_sensor[3];

        uint16_t voltage_sensor;

        uint32_t hc_pulse_duration;

        int16_t acceleration[3];
        int16_t gyro[3];
        int16_t magnetometer[3];

        int16_t imu_temperature;

        uint16_t lidar_speed;
    };

    struct ReadingsConverted
    {
        float ground_sensor_normalized[3];

        float voltage_sensor_V;

        float hc_distance_m;

        float acceleration_m_per_s2[3];
        float gyro_rad_per_s[3];
        float magnetometer_G[3];

        float pitch_rad;
        float roll_rad;
        float heading_rad;

        float imu_temperature_C;

        float lidar_speed_rpm;
    };

    ReadingsRaw raw;
    ReadingsConverted converted;
};