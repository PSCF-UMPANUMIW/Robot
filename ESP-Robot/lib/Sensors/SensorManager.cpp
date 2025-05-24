#include <Arduino.h>
#include <SensorManager.hpp>
#include <EspNowClient.hpp>

void SensorManager::sendPayload()
{
    EspNowClient::instance().sendMessage(payloadSensors);
}

void SensorManager::print()
{
    for (int i = 0; i < 3; ++i)
        Serial.printf("Ground sensor %d: %.2f (%d)\n", i, payloadSensors.converted.ground_sensor_normalized[i], payloadSensors.raw.ground_sensor[i]);
    
    Serial.printf("Battery voltage: %.2fV (%d)\n", payloadSensors.converted.voltage_sensor_V, payloadSensors.raw.voltage_sensor);

    Serial.printf("HC distance: %.2fm (%d)\n", payloadSensors.converted.hc_distance_m, payloadSensors.raw.hc_pulse_duration);

    Serial.printf("Acceleration: [%.2f, %.2f, %.2f] (%d, %d, %d)\n",
        payloadSensors.converted.acceleration_m_per_s2[0], payloadSensors.converted.acceleration_m_per_s2[1], payloadSensors.converted.acceleration_m_per_s2[2],
        payloadSensors.raw.acceleration[0], payloadSensors.raw.acceleration[1], payloadSensors.raw.acceleration[2]);
    
    Serial.printf("Gyro: [%.2f, %.2f, %.2f] (%d, %d, %d)\n",
        payloadSensors.converted.gyro_rad_per_s[0], payloadSensors.converted.gyro_rad_per_s[1], payloadSensors.converted.gyro_rad_per_s[2],
        payloadSensors.raw.gyro[0], payloadSensors.raw.gyro[1], payloadSensors.raw.gyro[2]);
    
    Serial.printf("Magnetometer: [%.2f, %.2f, %.2f] (%d, %d, %d)\n",
        payloadSensors.converted.magnetometer_G[0], payloadSensors.converted.magnetometer_G[1], payloadSensors.converted.magnetometer_G[2],
        payloadSensors.raw.magnetometer[0], payloadSensors.raw.magnetometer[1], payloadSensors.raw.magnetometer[2]);
    
    Serial.printf("IMU temperature: %.2fC (%d)\n", payloadSensors.converted.imu_temperature_C, payloadSensors.raw.imu_temperature);

    Serial.printf("Lidar speed: %.2frpm (%d)\n", payloadSensors.converted.lidar_speed_rpm, payloadSensors.raw.lidar_speed);

    Serial.println();
}

void SensorManager::printForTeleplot()
{
    for (int i = 0; i < 3; ++i)
        Serial.printf(">ground_%d:%f\n", i, payloadSensors.converted.ground_sensor_normalized[i]);
    
    Serial.printf(">batt_v:%f\n", payloadSensors.converted.voltage_sensor_V);

    Serial.printf(">front_dist:%f\n", payloadSensors.converted.hc_distance_m);

    Serial.printf(">a_x:%f\n>a_y:%f\n>a_z:%f\n",
        payloadSensors.converted.acceleration_m_per_s2[0], payloadSensors.converted.acceleration_m_per_s2[1], payloadSensors.converted.acceleration_m_per_s2[2]);
    
    Serial.printf(">gyro_x:%f\n>gyro_y:%f\n>gyro_z:%f\n",
        payloadSensors.converted.gyro_rad_per_s[0], payloadSensors.converted.gyro_rad_per_s[1], payloadSensors.converted.gyro_rad_per_s[2]);
    
    Serial.printf(">mag_x:%f\n>mag_y:%f\n>mag_z:%f\n",
        payloadSensors.converted.magnetometer_G[0], payloadSensors.converted.magnetometer_G[1], payloadSensors.converted.magnetometer_G[2]);
    
    Serial.printf(">pitch:%f\n>roll:%f\n>heading:%f\n",
        payloadSensors.converted.pitch_rad, payloadSensors.converted.roll_rad, payloadSensors.converted.heading_rad);
    
    Serial.printf(">imu_temp:%f\n", payloadSensors.converted.imu_temperature_C);

    Serial.printf(">lidar_speed:%f\n", payloadSensors.converted.lidar_speed_rpm);

    Serial.println();
}