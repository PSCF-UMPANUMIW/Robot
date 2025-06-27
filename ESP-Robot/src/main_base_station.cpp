#include <Arduino.h>

#include "constants.hpp"

#include <EspNowClient.hpp>
#include <Payloads/PayloadSensors.hpp>
#include <Payloads/PayloadMotorConfig.hpp>
#include <Payloads/PayloadMoveCommand.hpp>
#include <Payloads/PayloadLidarConfig.hpp>

#include <SensorManager.hpp>

HardwareSerial& Server = Serial1;
HardwareSerial& Debug = Serial;

static PayloadMotorConfig motorConfig {
    .speed = SettingsLevel::LEVEL_LOW,
    .acceleration = SettingsLevel::LEVEL_LOW,
    .current = SettingsLevel::LEVEL_LOW,
    .enableSteppers = true
};

enum class MoveType : bool
{
    LINEAR = false,
    ROTATION = true,
};

void sendMove(MoveType type, float value)
{
    PayloadMoveCommand comm {
        .isRotation = static_cast<bool>(type),
        .value = value
    };

    EspNowClient::sendMessage(comm);
}

void sendMotorConf()
{
    EspNowClient::sendMessage(motorConfig);
}

void sendLidarConf(LidarReader::MotorSpeed speed)
{
    PayloadLidarConfig conf {
        .motorSpeed = speed,
        .enableLaser = speed != LidarReader::MotorSpeed::OFF
    };

    EspNowClient::sendMessage(conf);
}

void setupClient()
{
    EspNowClient::begin();
    EspNowClient::addPeer(ROBOT_MAC_ADDR);

    EspNowClient::registerPayloadHandler<PayloadSensors>([](PayloadSensors const& payload) {
        SensorManager::print(payload, Server);
    });
}

void serialHandler();

void setup()
{
    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1, 19, 18);

    setupClient();
    sendMotorConf();
    sendLidarConf(LidarReader::MotorSpeed::OFF);

    Server.onReceive(serialHandler);

    Debug.println("Base Station Setup Complete");
}

void loop()
{
    // in order not to let the CPU go to waste,
    // we will calculate PI in the spare time

    static double pi = 0.0;
    static unsigned long n = 0;

    pi += (n % 2 == 0 ? 4.0 : -4.0) / (2 * n + 1);
    n++;

    delay(100); // no rush
}

void serialHandler()
{
    if (Server.available() == 0)
        return;
        
    char command = Server.read();
    int value = Server.parseInt();

    while (Server.available())
        Server.read();

    Debug.printf("Received command '%c' with value %d\n", command, value);

    switch (command)
    {
        case 'M': // move
            sendMove(MoveType::LINEAR, 0.001f * value); // mm to m
            break;
        case 'R': // rotate
            sendMove(MoveType::ROTATION, 0.001f * value); // mrad to rad
            break;
        case 'a': // acceleration
            motorConfig.acceleration = static_cast<SettingsLevel>(value);
            sendMotorConf();
            break;
        case 's': // speed
            motorConfig.speed = static_cast<SettingsLevel>(value);
            sendMotorConf();
            break;
        case 'c': // current
            motorConfig.current = static_cast<SettingsLevel>(value);
            sendMotorConf();
            break;
        case 'l': // lidar
            sendLidarConf(static_cast<LidarReader::MotorSpeed>(value));
            break;
        default:
            Debug.printf("Unknown command '%c'\n", command);
            break;
    }
}