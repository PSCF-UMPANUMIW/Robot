#include <Arduino.h>

#include "pins.hpp"
#include "constants.hpp"

#include <EspNowClient.hpp>
#include <Payloads/PayloadMoveCommand.hpp>
#include <Payloads/PayloadMotorConfig.hpp>

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


static LidarReader* lidarReader;
static ISensor* sensors[NUM_SENSORS] {};
static WheelPlatform* platform;


void setupLidar()
{
    lidarReader = new LidarReader(SERIAL_LIDAR, LIDAR_PINS.data, LIDAR_PINS.enable, LIDAR_PINS.motor_enable);

    auto lidarCallback = [](LidarMeasurement const& m) {
        static LidarMeasurementBuffer buffer;
        buffer.add(m);
    };

    lidarReader->begin();
    lidarReader->setDataReadyCallback(lidarCallback);
    lidarReader->enableLaser();
    lidarReader->setMotorSpeed(LidarReader::MotorSpeed::LOW_SPEED);
}

void setupMotors()
{
    Serial2.begin(115200, SERIAL_8N1, TMC_SERIAL_PINS.rx, TMC_SERIAL_PINS.tx);

    TMC2209Stepper* driverL = new TMC2209Stepper(&SERIAL_TMC, TMC_R_SENSE, 0b00);
    TMC2209Stepper* driverR = new TMC2209Stepper(&SERIAL_TMC, TMC_R_SENSE, 0b01);

    driverL->begin();
    driverR->begin();

    driverL->mstep_reg_select(true);
    driverR->mstep_reg_select(true);

    driverL->microsteps(0);
    driverR->microsteps(0);

    AccelStepper* stepperL = new AccelStepper(AccelStepper::DRIVER, STEPPER_L_PINS.step, STEPPER_L_PINS.dir);
    AccelStepper* stepperR = new AccelStepper(AccelStepper::DRIVER, STEPPER_R_PINS.step, STEPPER_R_PINS.dir);

    stepperL->setEnablePin(STEPPER_L_PINS.enable);
    stepperR->setEnablePin(STEPPER_L_PINS.enable);

    stepperL->setPinsInverted(false, false, true);
    stepperR->setPinsInverted(true, false, true);

    stepperL->setCurrentPosition(0);
    stepperR->setCurrentPosition(0);

    ManagedStepper* managedStepperL = new ManagedStepper(*stepperL, *driverL, ROBOT_STEPS_PER_METER);
    ManagedStepper* managedStepperR = new ManagedStepper(*stepperR, *driverR, ROBOT_STEPS_PER_METER);

    platform = new WheelPlatform(*managedStepperL, *managedStepperR, ROBOT_WHEELBASE);
    platform->begin();
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
                platform->turn(payload.value);
            else
                platform->move(payload.value);
        }
    );

    client.registerPayloadHandler<PayloadMotorConfig>(
        [](PayloadMotorConfig const& payload)
        {
            Serial.printf("Received motor config command: speed: %d\tacceleration:%d\tcurrent:%d\n",
                payload.speed, payload.acceleration, payload.current);

            platform->setMaxSpeed(payload.speed);
            platform->setAcceleration(payload.acceleration);
            platform->setDriverCurrent(payload.current);

            if (payload.enableSteppers)
                platform->enableSteppers();
            else
                platform->disableSteppers();
        }
    );
}

void setupSensors()
{
    sensors[0] = new GroundSensor(GroundSensor::Position::LEFT,  IR_SENSOR_PINS[0].analog_in, 0, 4095);
    sensors[1] = new GroundSensor(GroundSensor::Position::FRONT, IR_SENSOR_PINS[1].analog_in, 0, 4095);
    sensors[2] = new GroundSensor(GroundSensor::Position::RIGHT, IR_SENSOR_PINS[2].analog_in, 0, 4095);
    sensors[3] = new ProximitySensor(HC_SR04_PINS.trig, HC_SR04_PINS.echo, 1.f);
    sensors[4] = new VoltageSensor(VOLTAGE_SENSOR_PINS.analog_in, -6.841e-7f, 7.923e-3f, -2.629f);
    sensors[5] = new IMU6AxisSensor(Wire, 0x68);
    sensors[6] = new MagnetometerSensor(Wire, MagnetometerSensor::Calibration{ 0.248f, 0.672f, -0.203f, 0.210f, 0.0f, 0.0f});
}

void createSensorReadTask()
{
    xTaskCreatePinnedToCore(
        [] (void*) {
            while (true)
            {
                constexpr int period_ms = 25;

                auto start = millis();

                for (auto sensor : sensors)
                    sensor->read();

                auto elapsed = millis() - start;
                
                delay(period_ms - elapsed);
            }
        },
        "Sensor data collection", 8192, NULL, 2, NULL, 0
    );
}

void createSensorSendTask()
{
    xTaskCreatePinnedToCore(
        [] (void*) {
            while (true)
            {
                delay(100);
                SensorManager::instance().sendPayload();
            }
        },
        "Sensor send task", 8192, NULL, 1, NULL, 1
    );
}

void setup()
{
    Serial.begin(115200);
    Wire.begin(I2C_PINS.sda, I2C_PINS.scl);
    
    setupEspNow();
    setupMotors();
    setupLidar();
    setupSensors();

    createSensorReadTask();
    createSensorSendTask();

    Serial.println("Robot Setup Complete");
}


void loop()
{
    lidarReader->readData();
    platform->run();
}
