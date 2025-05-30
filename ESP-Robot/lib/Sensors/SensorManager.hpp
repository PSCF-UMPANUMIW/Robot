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

    static PayloadSensors::ReadingsRaw& getRaw() { return instance().payloadSensors.raw; }

    static PayloadSensors::ReadingsConverted& getConverted() { return instance().payloadSensors.converted; }

    static void sendPayload();

    static void print();
    static void print(PayloadSensors const& payload);

private:
    SensorManager() = default;
    ~SensorManager() = default;

    SensorManager(const SensorManager&) = delete;
    SensorManager(SensorManager&&) = delete;
    SensorManager& operator=(const SensorManager&) = delete;
    SensorManager& operator=(SensorManager&&) = delete;

    PayloadSensors payloadSensors;
};