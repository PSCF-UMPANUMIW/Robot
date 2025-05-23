#pragma once

#include <LidarMeasurement.hpp>
#include <EspNowClient.hpp>
#include <Payloads/PayloadLidar.hpp>

class LidarMeasurementBuffer
{
public:
    LidarMeasurementBuffer() = default;

    void add(LidarMeasurement const& measurement)
    {
        payload.measurements[idx++] = measurement;

        if (idx >= PayloadLidar::MEASUREMENTS_PER_PACKET)
        {
            EspNowClient::instance().sendMessage(payload);
            idx = 0;
        }
    }

private:
    PayloadLidar payload;
    size_t idx = 0;
};