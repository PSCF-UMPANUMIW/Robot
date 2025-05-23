#include <Arduino.h>
#include <Wire.h>

#include "pins.hpp"
#include "constants.hpp"

#include <LidarReader.hpp>
#include <LidarMeasurementBuffer.hpp>

#include <EspNowClient.hpp>
#include <Payloads/PayloadTraits.hpp>

#include <SensorManager.hpp>
#include <GroundSensor.hpp>
#include <ProximitySensor.hpp>
#include <VoltageSensor.hpp>
#include <IMU6AxisSensor.hpp>
#include <MagnetometerSensor.hpp>

static LidarReader lidarReader(
    Serial1,
    LIDAR_PINS.data,
    LIDAR_PINS.enable,
    LIDAR_PINS.motor_enable
);

static GroundSensor  groundSensorLeft(GroundSensor::Position::LEFT,  IR_SENSOR_PINS[0].analog_in, 0, 4095);
static GroundSensor groundSensorFront(GroundSensor::Position::FRONT, IR_SENSOR_PINS[1].analog_in, 0, 4095);
static GroundSensor groundSensorRight(GroundSensor::Position::RIGHT, IR_SENSOR_PINS[2].analog_in, 0, 4095);

static ProximitySensor proximitySensor(HC_SR04_PINS.trig, HC_SR04_PINS.echo, 1.f);

static VoltageSensor voltageSensor(VOLTAGE_SENSOR_PINS.analog_in, -6.841e-7f, 7.923e-3f, -2.629f);

static IMU6AxisSensor imu(Wire);

static MagnetometerSensor magnetometer(Wire, 
    MagnetometerSensor::Calibration{
        .xmin = 0.248f,
        .xmax = 0.672f,
        .ymin = -0.203f,
        .ymax = 0.210f,
        .zmin = 0.0f,
        .zmax = 0.0f
    }
);

static ISensor* sensors[] = {
    &groundSensorLeft,
    &groundSensorFront,
    &groundSensorRight,
    &proximitySensor,
    &voltageSensor,
    &imu,
    &magnetometer
};

#include <TMCStepper.h>
#include <AccelStepper.h>

#define TMC_SERIAL_PORT Serial2
#define DRIVER_L_ADDRESS 0b00
#define DRIVER_R_ADDRESS 0b01
#define R_SENSE 0.11f

static TMC2209Stepper driverL(&TMC_SERIAL_PORT, R_SENSE, DRIVER_L_ADDRESS);
static TMC2209Stepper driverR(&TMC_SERIAL_PORT, R_SENSE, DRIVER_R_ADDRESS);

static AccelStepper stepperL(AccelStepper::DRIVER, STEPPER_L_PINS.step, STEPPER_L_PINS.dir);
static AccelStepper stepperR(AccelStepper::DRIVER, STEPPER_R_PINS.step, STEPPER_R_PINS.dir);


void setupLidar()
{
    auto lidarCallback = [](LidarMeasurement const& m) {
        static LidarMeasurementBuffer buffer;
        buffer.add(m);
    };

    lidarReader.begin();
    lidarReader.setDataReadyCallback(lidarCallback);
    lidarReader.enableLaser();
    lidarReader.setMotorSpeed(LidarReader::MotorSpeed::OFF);
}

void setupMotors()
{
    Serial2.begin(115200, SERIAL_8N1, STEPPER_L_PINS.rx, STEPPER_L_PINS.tx);

    driverL.begin();
    driverR.begin();

    driverL.rms_current(1200);
    driverL.microsteps(0);

    driverR.rms_current(1200);
    driverR.microsteps(0);
    
    stepperL.setEnablePin(14);
    stepperL.setPinsInverted(false, false, true);

    stepperR.setEnablePin(14);
    stepperR.setPinsInverted(true, false, true);

    stepperL.setMaxSpeed(800);
    stepperL.setAcceleration(400);
    stepperL.setCurrentPosition(0);

    stepperR.setMaxSpeed(800);
    stepperR.setAcceleration(400);
    stepperR.setCurrentPosition(0);

    // stepperL.enableOutputs();
    // stepperR.enableOutputs();
}

void setupEspNow()
{
    auto& client = EspNowClient::instance();
    client.begin();
    client.addPeer(BASE_STATION_MAC_ADDR);

    client.registerPayloadHandler<PayloadMoveCommand>(
        [](PayloadMoveCommand const& payload)
        {
            stepperL.move(payload.distanceL);
            stepperR.move(payload.distanceR);
        }
    );

    client.registerPayloadHandler<PayloadMotorConfig>(
        [](PayloadMotorConfig const& payload)
        {
            driverL.rms_current(payload.rmsCurrent_mA);
            driverL.microsteps(payload.microsteps);
            stepperL.setMaxSpeed(payload.maxSpeed);
            stepperL.setAcceleration(payload.maxAcceleration);

            driverR.rms_current(payload.rmsCurrent_mA);
            driverR.microsteps(payload.microsteps);
            stepperR.setMaxSpeed(payload.maxSpeed);
            stepperR.setAcceleration(payload.maxAcceleration);
        }
    );
}

void setupSensors()
{
    for (auto sensor : sensors)
        sensor->begin();
}

void setup()
{
    Serial.begin(115200);
    
    setupEspNow();
    setupMotors();
    setupLidar();

    Wire.begin(I2C_PINS.sda, I2C_PINS.scl);

    setupSensors();

    xTaskCreatePinnedToCore(
        [] (void*) {
            while (true)
            {
                constexpr int period_ms = 20;

                for (auto sensor : sensors)
                    sensor->read();

                delay(period_ms);
            }
        },
        "Sensor data collection", 8192, NULL, 2, NULL, 1
    );

    xTaskCreatePinnedToCore(
        [] (void*) {
            while (true)
            {
                delay(100);
                SensorManager::instance().printForTeleplot();
            }
        },
        "Sensor raport", 8192, NULL, 1, NULL, 1
    );

    Serial.println("Robot Setup Complete");
}



void loop()
{
    lidarReader.readData();

    // stepperL.run();
    // stepperR.run();
}