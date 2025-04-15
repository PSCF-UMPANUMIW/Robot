#pragma once

#include "LidarMeasurement.hpp"
#include "ESP_NOW_Payloads/PayloadLidar.hpp"
#include "EspNowClient.hpp"

class LidarMeasurementBuffer
{
public:
    LidarMeasurementBuffer() = default;

    void add(LidarMeasurement const& measurement)
    {
        payload.measurements[idx++] = measurement;

        if (idx >= PayloadLidar::MEASUREMENTS_PER_PACKET)
        {
            EspNowClient::sendMessage(payload);
            idx = 0;
        }
    }

private:
    PayloadLidar payload;
    size_t idx = 0;
};