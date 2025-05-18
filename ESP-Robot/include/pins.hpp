#pragma once

#include <cstdint>

// ====================================================

struct lidar_pins_t
{
    uint8_t enable, motor_enable, data;
};

constexpr lidar_pins_t LIDAR_PINS = {
    .enable = 16,
    .motor_enable = 17,
    .data = 18
};

// ====================================================

struct i2c_pins_t
{
    uint8_t sda, scl;
};

constexpr i2c_pins_t I2C_PINS = {
    .sda = 33,
    .scl = 32
};

// ====================================================

struct stepper_pins_t
{
    uint8_t step, dir, enable, rx, tx;
};

constexpr stepper_pins_t STEPPER_L_PINS= {
    .step = 26,
    .dir = 25,
    .enable = 14,
    .rx = 13,
    .tx = 27
};

constexpr stepper_pins_t STEPPER_R_PINS= {
    .step = 4,
    .dir = 15,
    .enable = 14,
    .rx = 13,
    .tx = 27
};

// ====================================================

struct ir_sensor_pins_t
{
    uint8_t analog_in;
};

constexpr ir_sensor_pins_t IR_SENSOR_PINS[] = {
    { .analog_in = 36 },
    { .analog_in = 39 },
    { .analog_in = 34 },
};

// ====================================================

struct hc_sr04_pins_t
{
    uint8_t trig, echo;
};

constexpr hc_sr04_pins_t HC_SR04_PINS = {
    .trig = 23,
    .echo = 22 
};

// ====================================================

struct voltage_sensor_pins_t
{
    uint8_t analog_in;
};

constexpr voltage_sensor_pins_t VOLTAGE_SENSOR_PINS = {
    .analog_in = 35
};

// ====================================================

struct raspberry_pi_pins_t
{
    uint8_t a, b;
};

constexpr raspberry_pi_pins_t RASPBERRY_PI_PINS = {
    .a = 21,
    .b = 19
};