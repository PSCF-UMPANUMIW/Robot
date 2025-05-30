#include <Arduino.h>
#include <SensorManager.hpp>
#include <EspNowClient.hpp>

void SensorManager::sendPayload()
{
    EspNowClient::instance().sendMessage(instance().payloadSensors);
}

void SensorManager::print()
{
    print(instance().payloadSensors);
}

void SensorManager::print(PayloadSensors const& payload)
{
    for (int i = 0; i < 3; ++i)
        Serial.printf(">ground_%d:%f\n", i, payload.converted.ground_sensor_normalized[i]);
    
    Serial.printf(">batt_v:%f\n", payload.converted.voltage_sensor_V);

    Serial.printf(">front_dist:%f\n", payload.converted.hc_distance_m);

    Serial.printf(">a_x:%f\n>a_y:%f\n>a_z:%f\n",
        payload.converted.acceleration_m_per_s2[0], payload.converted.acceleration_m_per_s2[1], payload.converted.acceleration_m_per_s2[2]);
    
    Serial.printf(">gyro_x:%f\n>gyro_y:%f\n>gyro_z:%f\n",
        payload.converted.gyro_rad_per_s[0], payload.converted.gyro_rad_per_s[1], payload.converted.gyro_rad_per_s[2]);
    
    Serial.printf(">mag_x:%f\n>mag_y:%f\n>mag_z:%f\n",
        payload.converted.magnetometer_G[0], payload.converted.magnetometer_G[1], payload.converted.magnetometer_G[2]);
    
    Serial.printf(">pitch:%f\n>roll:%f\n>heading:%f\n",
        payload.converted.pitch_rad, payload.converted.roll_rad, payload.converted.heading_rad);
    
    Serial.printf(">imu_temp:%f\n", payload.converted.imu_temperature_C);

    Serial.printf(">lidar_speed:%f\n", payload.converted.lidar_speed_rpm);

    Serial.println();
}