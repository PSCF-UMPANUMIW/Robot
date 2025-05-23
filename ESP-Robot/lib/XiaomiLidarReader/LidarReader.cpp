#include <LidarReader.hpp>
#include <LidarMeasurement.hpp>

#include <SensorManager.hpp>

LidarReader::LidarReader(HardwareSerial& serial, uint8_t rxPin, uint8_t enPin, uint8_t motorEnPin)
    : uart(serial), rxPin(rxPin), enPin(enPin), motorEnPin(motorEnPin)
{}


void LidarReader::begin()
{
    uart.begin(115200, SERIAL_8N1, rxPin, -1);
    pinMode(enPin, OUTPUT);
    pinMode(motorEnPin, OUTPUT);
}


void LidarReader::setMotorSpeed(MotorSpeed speed)
{
    switch (speed)
    {
        case MotorSpeed::OFF:
            analogWrite(motorEnPin, 0);
            break;
        case MotorSpeed::LOW_SPEED:
            analogWrite(motorEnPin, 150); // these are also just guesses, please characterize this
            break;
        case MotorSpeed::HIGH_SPEED:
            analogWrite(motorEnPin, 220); // these are also just guesses, please characterize this
            break;
        case MotorSpeed::TOO_FAST:
            analogWrite(motorEnPin, 255);
            break;
    }
}


void LidarReader::readData()
{
    while (uart.available())
    {
        const byte b = uart.read();

        if (b == HEAD_BYTE)
        {
            buffer.clear();
        }

        buffer.push(b);

        if (buffer.isFull())
        {
            LidarMeasurement lp = decodeMeasurement();
            buffer.clear();
            if (callback) callback(lp);
        }
    }
}


LidarMeasurement LidarReader::decodeMeasurement() const
{   
    LidarMeasurement lp;
    
    lp.startAngle = (buffer[1] - 0xA0) * 4;

    const uint16_t speedRaw = ((uint16_t)buffer[3] << 8) | buffer[2];
    auto& sm = SensorManager::instance();
    sm.getRaw().lidar_speed = speedRaw;
    sm.getConverted().lidar_speed_rpm = (float)speedRaw * 60.0f / 360.0f; // this is a guess, please characterize this
    
    for (int i = 0; i < DISTANCES_PER_PACKET; i++)
    {
        int pi = 4 + 4 * i;
        lp.distance[i] = (((uint16_t)buffer[pi + i] & 0x3D) << 8) | buffer[pi];
    }
    
    return lp;
}