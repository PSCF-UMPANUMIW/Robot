#include <Arduino.h>

#include "pins.hpp"
#include "constants.hpp"
#include "printmac.hpp"

#include <EspNowClient.hpp>
#include <Payloads/PayloadTraits.hpp>

#include <LidarReader.hpp>
#include <LidarMeasurementBuffer.hpp>

#include <SensorManager.hpp>
#include <GroundSensor.hpp>
#include <ProximitySensor.hpp>
#include <VoltageSensor.hpp>
#include <Wire.h>
#include <IMU6AxisSensor.hpp>
#include <MagnetometerSensor.hpp>

#include <TMCStepper.h>
#include <AccelStepper.h>
#include <ManagedStepper.hpp>
#include <WheelPlatform.hpp>

#pragma region lidar

static LidarReader lidarReader(
    SERIAL_LIDAR,
    LIDAR_PINS.data,
    LIDAR_PINS.enable,
    LIDAR_PINS.motor_enable
);

#pragma endregion

#pragma region sensors

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

#pragma endregion

#pragma region motors

static TMC2209Stepper driverL(&SERIAL_TMC, TMC_R_SENSE, 0b00);
static TMC2209Stepper driverR(&SERIAL_TMC, TMC_R_SENSE, 0b01);

static AccelStepper stepperL(AccelStepper::DRIVER, STEPPER_L_PINS.step, STEPPER_L_PINS.dir);
static AccelStepper stepperR(AccelStepper::DRIVER, STEPPER_R_PINS.step, STEPPER_R_PINS.dir);

static ManagedStepper managedStepperL(stepperL, driverL, ROBOT_STEPS_PER_METER);
static ManagedStepper managedStepperR(stepperR, driverR, ROBOT_STEPS_PER_METER);

static WheelPlatform platform(managedStepperL, managedStepperR, 0.284f);

#pragma endregion

void setupLidar()
{
    auto lidarCallback = [](LidarMeasurement const& m) {
        static LidarMeasurementBuffer buffer;
        buffer.add(m);
    };

    lidarReader.begin();
    lidarReader.setDataReadyCallback(lidarCallback);
    // lidarReader.enableLaser();
    // lidarReader.setMotorSpeed(LidarReader::MotorSpeed::LOW_SPEED);
}

void setupMotors()
{
    driverL.begin();
    driverR.begin();

    delay(1000);

    driverL.mstep_reg_select(true);
    driverR.mstep_reg_select(true);

    driverL.microsteps(0);
    driverR.microsteps(0);

    stepperL.setEnablePin(STEPPER_L_PINS.enable);
    stepperR.setEnablePin(STEPPER_L_PINS.enable);

    stepperL.setPinsInverted(false, false, true);
    stepperR.setPinsInverted(true, false, true);

    stepperL.setCurrentPosition(0);
    stepperR.setCurrentPosition(0);

    platform.begin();
}

void setupEspNow()
{
    auto& client = EspNowClient::instance();
    client.begin();
    client.addPeer(BASE_STATION_MAC_ADDR);

    client.registerPayloadHandler<PayloadMoveCommand>(
        [](PayloadMoveCommand const& payload)
        {
            Serial.printf("Received move command: %s by %f\n", payload.isRotation ? "rotate" : "move", payload.value);
            if (payload.isRotation)
            {
                platform.turn(payload.value);
            }
            else
            {
                platform.move(payload.value);
            }
        }
    );

    client.registerPayloadHandler<PayloadMotorConfig>(
        [](PayloadMotorConfig const& payload)
        {
            Serial.printf("Received motor config command: speed: %d\tacceleration:%d\tcurrent:%d\n",
                payload.speed, payload.acceleration, payload.current);

            platform.setMaxSpeed(payload.speed);
            platform.setAcceleration(payload.acceleration);
            platform.setDriverCurrent(payload.current);

            if (payload.enableSteppers)
                platform.enableSteppers();
            else
                platform.disableSteppers();
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
        "Sensor data collection", 8192, NULL, 2, NULL, 0
    );

    // xTaskCreatePinnedToCore(
    //     [] (void*) {
    //         while (true)
    //         {
    //             delay(100);
                
    //             SensorManager::instance().sendPayload();
    //         }
    //     },
    //     "Sensor raport", 8192, NULL, 1, NULL, 1
    // );

    xTaskCreatePinnedToCore(
        [] (void*) {
            while (true)
            {
                delay(500);
                
                Serial.printf("currents: %i %i\n", driverL.rms_current(), driverR.rms_current());
                Serial.printf("microsteps: %i %i\n\n", driverL.microsteps(), driverR.microsteps());
            }
        },
        "Motor raport", 8192, NULL, 1, NULL, 1
    );


    printMacAddress();

    Serial.println("Robot Setup Complete");
}



void loop()
{
    lidarReader.readData();

    platform.run();
}


