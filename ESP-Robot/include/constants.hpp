#pragma once

#include <Arduino.h>

constexpr uint8_t ROBOT_MAC_ADDR[] = {0xC4, 0xD8, 0xD5, 0xBE, 0x92, 0xB4};
constexpr uint8_t BASE_STATION_MAC_ADDR[] = {0xCC, 0x7B, 0x5C, 0xF0, 0x95, 0x40};
constexpr uint8_t BROADCAST_MAC_ADDR[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

HardwareSerial& SERIAL_LIDAR = Serial1;
HardwareSerial& SERIAL_TMC   = Serial2;

constexpr int NUM_SENSORS = 7;

constexpr float TMC_R_SENSE = 0.11f;

constexpr float ROBOT_WHEELBASE = 0.284f;

constexpr float ROBOT_WHEEL_DIAMETER = 0.073f;
constexpr float ROBOT_WHEEL_CIRCUMFERENCE = PI * ROBOT_WHEEL_DIAMETER;
constexpr long  ROBOT_STEPS_PER_REVOLUTION = 200;
constexpr float ROBOT_STEPS_PER_METER = ROBOT_STEPS_PER_REVOLUTION / ROBOT_WHEEL_CIRCUMFERENCE;