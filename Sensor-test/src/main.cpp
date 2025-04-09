#include <Arduino.h>
#include <Wire.h>

#include "pins.hpp"
#include "constants.hpp"

#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include "HC_SR04.hpp"
#include "VoltageDividerSensor.hpp"

static Adafruit_MPU6050 mpu;
static HC_SR04 hc(HC_PINS.trig, HC_PINS.echo);
static VoltageDividerSensor voltageSensor(VOLTAGE_IN_PIN, VOLTAGE_SENSOR_MULTIPLIER);

void initMPU()
{
    if (!mpu.begin())
    {
        Serial.println("MPU6050 not found");
        while (1)
            delay(10);
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
    mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void setup()
{
    Serial.begin(115200);
    Wire.begin(I2C_PINS.sda, I2C_PINS.scl);
    delay(1000);

    initMPU();
    hc.begin();
    voltageSensor.init();

    for (int i = 0; i < NUM_IR_SENSORS; ++i)
    {
        pinMode(IR_SENSORS_PINS[i], INPUT);
    }
}

void loop()
{
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    Serial.printf(">ax:%f\n>ay:%f\n>az:%f\n", a.acceleration.x, a.acceleration.y, a.acceleration.z);
    Serial.printf(">gx:%f\n>gy:%f\n>gz:%f\n", g.gyro.x, g.gyro.y, g.gyro.z);
    Serial.printf(">temp:%f\n", temp.temperature);

    Serial.printf(">distance:%f\n", hc.getDistance());

    Serial.printf(">voltage:%f\n", voltageSensor.getVoltage());

    for (int i = 0; i < NUM_IR_SENSORS; ++i)
    {
        Serial.printf(">ir_sensor_%d:%d\n", i, analogRead(IR_SENSORS_PINS[i]));
    }

    delay(100);
}