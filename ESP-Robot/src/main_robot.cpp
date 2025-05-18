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

void w()
{
    stepperL.move(100);
    stepperR.move(100);
}

void s()
{
    stepperL.move(-100);
    stepperR.move(-100);
}

void a()
{
    stepperL.move(-100);
    stepperR.move(100);
}

void d()
{
    stepperL.move(100);
    stepperR.move(-100);
}

void setup()
{
    Serial.begin(115200);
    
    EspNowClient::init();
    EspNowClient::addPeer(BASE_STATION_MAC_ADDR);

    EspNowClient::registerPayloadHandler<PayloadMoveCommand>([](void* payload) {
        
        PayloadMoveCommand& p = *static_cast<PayloadMoveCommand*>(payload);

        Serial.printf("Received move command: L: %d, R: %d\n", p.distanceL, p.distanceR);
        // stepperL.move(payload.distanceL);
        // stepperR.move(payload.distanceR);
    });
    
    // Serial1.begin(115200, SERIAL_8N1, LIDAR_TX, -1);
    // lidarReader = new LidarReader(Serial1, onLidarMeasurement);

    Serial2.begin(115200, SERIAL_8N1, STEPPER_L_PINS.rx, STEPPER_L_PINS.tx);

    driverL.begin();
    driverR.begin();

    driverL.rms_current(1200); // set motor RMS current
    driverL.microsteps(0); // set microsteps to 16

    driverR.rms_current(1200); // set motor RMS current
    driverR.microsteps(0); // set microsteps to 16

    // read back data from driver
    Serial.print("Driver L: ");
    Serial.print("rms_current: "); Serial.println(driverL.rms_current(), DEC);
    Serial.print("microsteps: "); Serial.println(driverL.microsteps(), DEC);

    Serial.print("Driver R: ");
    Serial.print("rms_current: "); Serial.println(driverR.rms_current(), DEC);
    Serial.print("microsteps: "); Serial.println(driverR.microsteps(), DEC);

    
    stepperL.setEnablePin(14);
    stepperL.setPinsInverted(false, false, true);

    stepperR.setEnablePin(14);
    stepperR.setPinsInverted(true, false, true);

    stepperL.setMaxSpeed(400);
    stepperL.setAcceleration(400);
    stepperL.setCurrentPosition(0);

    stepperR.setMaxSpeed(400);
    stepperR.setAcceleration(400);
    stepperR.setCurrentPosition(0);

    stepperL.enableOutputs();
    stepperR.enableOutputs();



    // // ESPNOW receive callback
    // esp_now_register_recv_cb([](const uint8_t* mac_addr, const uint8_t* data, int data_len) {
    //     Serial.print("Received data: ");
    //     for (int i = 0; i < data_len; i++)
    //     {
    //         Serial.print(data[i], DEC);
    //         Serial.print(" ");
    //     }

    //     if (data_len != 1)
    //         return;
        
    //     switch (data[0])
    //     {
    //     case 1:
    //         w();
    //         break;
    //     case 2:
    //         s();
    //         break;
    //     case 3:
    //         a();
    //         break;
    //     case 4:
    //         d();
    //         break;
    //     }
    // });


    Serial.println("Robot Setup Complete");
}

void serialStepperDemo();

void loop()
{
    // lidarReader->readData();

    serialStepperDemo();
    
    stepperL.run();
    stepperR.run();
}

void serialStepperDemo()
{
    if (Serial.available())
    {
        char c = Serial.read();
        switch (c)
        {
            case 'i':
                w();
                break;
            case 'k':
                s();
                break;
            case 'j':
                a();
                break;
            case 'l':
                d();
                break;
        }
    }
}