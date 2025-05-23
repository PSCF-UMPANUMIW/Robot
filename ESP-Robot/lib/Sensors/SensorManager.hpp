#pragma once

#include <Payloads/PayloadSensors.hpp>
#include <EspNowClient.hpp>

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

    void sendPayload() { EspNowClient::instance().sendMessage(payloadSensors); }

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