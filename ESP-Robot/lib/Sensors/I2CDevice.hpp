#pragma once

#include <Arduino.h>
#include <Wire.h>

class I2CDevice
{
protected:
    I2CDevice(TwoWire& wire, uint8_t address)
        : wire(wire), address(address)
    {}

    void setByte(uint8_t reg, uint8_t value)
    {
        wire.beginTransmission(address);
        wire.write(reg);
        wire.write(value);
        wire.endTransmission();
    }

    void requestBytes(uint8_t reg, size_t count)
    {
        wire.beginTransmission(address);
        wire.write(reg);
        wire.endTransmission();
        wire.requestFrom(address, count);
    }

    int16_t readShortMSBfirst()
    {
        return (wire.read() << 8) | wire.read();
    }

    int16_t readShortLSBfirst()
    {
        return wire.read() | (wire.read() << 8);
    }

    
    TwoWire& wire;
    const uint8_t address;
};