#include <Arduino.h>
#include <Wire.h>

#include "pins.hpp"
#include "constants.hpp"

#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <DFRobot_QMC5883.h>

#include "HC_SR04.hpp"
#include "VoltageDividerSensor.hpp"
#include "ProximitySensor.hpp"


static HC_SR04 hc(HC_PINS.trig, HC_PINS.echo);
static VoltageDividerSensor voltageSensor(VOLTAGE_IN_PIN, VOLTAGE_SENSOR_MULTIPLIER);
static ProximitySensor irSensors[] = {
    ProximitySensor(IR_SENSORS_PINS[0], PROXIMITY_SENSOR_MIN, PROXIMITY_SENSOR_MAX),
    ProximitySensor(IR_SENSORS_PINS[1], PROXIMITY_SENSOR_MIN, PROXIMITY_SENSOR_MAX),
    ProximitySensor(IR_SENSORS_PINS[2], PROXIMITY_SENSOR_MIN, PROXIMITY_SENSOR_MAX),
};

static DFRobot_QMC5883 compass(&Wire, QMC5883_ADDRESS);
static Adafruit_MPU6050 mpu = Adafruit_MPU6050();

void scanI2C()
{
    Serial.println("Scanning I2C bus...");
    for (uint8_t address = 1; address < 127; address++)
    {
        Wire.beginTransmission(address);
        if (Wire.endTransmission() == 0)
        {
            Serial.printf("Found I2C device at address 0x%02X\n", address);
        }
    }
    Serial.println("I2C scan complete.");
}

int pins[] = {34,35,36,39};

void setup()
{
    // Serial.begin(115200);
    // Wire.begin(I2C_PINS.sda, I2C_PINS.scl);
    // delay(1000);

    // scanI2C();

    // if (!compass.begin())
    // {
    //     Serial.println("Failed to initialize QMC5883 compass!");
    //     while (1)
    //         delay(10);
    // }

    // hc.begin();
    // voltageSensor.init();
    
    // for (int i = 0; i < NUM_IR_SENSORS; ++i)
    //     irSensors[i].init();

    Serial.begin(115200);
    Serial1.begin(9600, SERIAL_8N1, 33, 32); 
}

void loop()
{

    // Serial.printf(">distance:%f\n", hc.getDistance());

    // Serial.printf(">voltage:%f\n", voltageSensor.getVoltage());

    // for (int i = 0; i < NUM_IR_SENSORS; ++i)
    // {
    //     Serial.printf(">ir_sensor_%d:%f\n", i, irSensors[i].getValue());
    // }

    sVector_t mag = compass.readRaw();

    float heading = atan2(mag.YAxis, mag.XAxis) * (180 / 3.14159265) + 180;
    if (heading < 0)
        heading += 360;
    
    if (heading > 360)
        heading -= 360;
    Serial.printf(">heading:%f\n", heading);

    if (Serial.available())
    {
        char c = Serial.read();
        Serial1.write(c);
    }

    if (Serial1.available())
    {
        char c = Serial1.read();
        Serial.write(c);
    }
    delay(10);
}