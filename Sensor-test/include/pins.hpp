#pragma once

#include <Arduino.h>

struct HC_SR04_Pins
{
    uint8_t trig;
    uint8_t echo;
};

constexpr HC_SR04_Pins HC_PINS = {18, 19};


struct I2C_Pins
{
    uint8_t sda;
    uint8_t scl;
};

constexpr I2C_Pins I2C_PINS = {21, 22};


constexpr uint8_t VOLTAGE_IN_PIN = 33;

constexpr uint8_t NUM_IR_SENSORS = 3;
constexpr uint8_t IR_SENSORS_PINS[NUM_IR_SENSORS] = {32, 35, 34};