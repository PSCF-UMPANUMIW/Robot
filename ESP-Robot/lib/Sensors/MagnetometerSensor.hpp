#pragma once

#include <ISensor.hpp>
#include <I2CDevice.hpp>
#include <SensorManager.hpp>
#include <Arduino.h>
#include <Wire.h>

// driver for the QMC5883L magnetometer
class MagnetometerSensor : public ISensor, public I2CDevice
{
public:
    struct Calibration
    {
        float xmin, xmax;
        float ymin, ymax;
        float zmin, zmax;
    };

    MagnetometerSensor(TwoWire& wire, Calibration cali, uint8_t address = 0x0D)
        : I2CDevice(wire, address)
    {
        offsets[0] = (cali.xmax - cali.xmin);
        offsets[1] = (cali.ymax - cali.ymin);
        offsets[2] = (cali.zmax - cali.zmin);
    }

    void begin() override
    {
        setByte(0x0B, 0x01);       // recommended value
        setByte(0x09, 0b00011001); // oversamling 512, 8G range, 100Hz, continuous mode
        setByte(0x0A, 0x00);       // default settings
    }

    void read() override
    {
        auto& raw = SensorManager::instance().getRaw();
        auto& converted = SensorManager::instance().getConverted();

        requestBytes(0x00, 6);
        for (int i = 0; i < 3; ++i)
        {
            raw.magnetometer[i] = readShortLSBfirst();
            converted.magnetometer_G[i] = raw.magnetometer[i] / 3000.f - offsets[i];
        }

        float mx = converted.magnetometer_G[0];
        float my = converted.magnetometer_G[1];
        float mz = converted.magnetometer_G[2];

        float heading = atan2(my, mx);
        if (heading < 0)
            heading += 2 * PI;
        heading = fmod(heading + 2 * PI, 2 * PI); // normalize to [0, 2*PI]
        
        updateWithExpFilter(converted.heading_rad, heading, 0.15f);
    }

private:
    float offsets[3];
};