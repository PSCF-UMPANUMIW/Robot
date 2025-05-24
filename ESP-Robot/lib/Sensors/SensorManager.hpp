#pragma once

#include "../EspNowClient/Payloads/PayloadSensors.hpp"
class EspNowClient;

class SensorManager
{
public:
    static SensorManager& instance()
    {
        static SensorManager instance;
        return instance;
    }

    PayloadSensors::ReadingsRaw& getRaw() { return payloadSensors.raw; }

    PayloadSensors::ReadingsConverted& getConverted() { return payloadSensors.converted; }

    void sendPayload();

    void print();
    void printForTeleplot();

private:
    SensorManager() = default;
    ~SensorManager() = default;

    SensorManager(const SensorManager&) = delete;
    SensorManager(SensorManager&&) = delete;
    SensorManager& operator=(const SensorManager&) = delete;
    SensorManager& operator=(SensorManager&&) = delete;

    PayloadSensors payloadSensors;
};