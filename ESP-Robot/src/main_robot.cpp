#include <Arduino.h>

#include "pins.hpp"
#include "constants.hpp"

#include <LidarReader.hpp>

#include "EspNowClient.hpp"
#include "ESP_NOW_Payloads/PayloadTraits.hpp"
#include "LidarMeasurementBuffer.hpp"


static LidarReader* lidarReader;


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

void onLidarMeasurement(LidarMeasurement const& m)
{
    static LidarMeasurementBuffer buffer;
    buffer.add(m);
}

void setup()
{
    Serial.begin(115200);
    
    EspNowClient::init();
    EspNowClient::addPeer(BASE_STATION_MAC_ADDR);

    EspNowClient::registerPayloadHandler<PayloadMoveCommand>(
        [](PayloadMoveCommand const& payload)
        {
            stepperL.move(payload.distanceL);
            stepperR.move(payload.distanceR);
        }
    );

    EspNowClient::registerPayloadHandler<PacketMotorConfig>(
        [](PacketMotorConfig const& payload)
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
    
    // Serial1.begin(115200, SERIAL_8N1, LIDAR_TX, -1);
    // lidarReader = new LidarReader(Serial1, onLidarMeasurement);

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

    stepperL.enableOutputs();
    stepperR.enableOutputs();


    Serial.println("Robot Setup Complete");
}



void loop()
{
    // lidarReader->readData();
    stepperL.run();
    stepperR.run();
}