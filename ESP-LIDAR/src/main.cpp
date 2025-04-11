#include <Arduino.h>

constexpr byte LIDAR_TX = 27;

constexpr byte HEAD_BYTE = 0xFA;
constexpr int PACKET_SIZE = 22;
constexpr int DISTANCES_PER_PACKET = 4;

static byte packet[PACKET_SIZE] = {};
static uint8_t packetIndex = 0;

void setup()
{
    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1, LIDAR_TX, -1); // RX, TX

    memset(packet, 0, sizeof(packet));
}

struct DecodedPacket
{
    uint16_t angle;
    uint16_t speed;
    uint16_t distance[DISTANCES_PER_PACKET];
};

DecodedPacket decodePacket()
{
    DecodedPacket dp;

    dp.angle = (packet[1] - 0xA0) * 4;

    dp.speed = ((uint16_t)packet[3] << 8) | packet[2];

    for (int i = 0; i < DISTANCES_PER_PACKET; i++)
    {
        int pi = 4 + 4 * i;
        dp.distance[i] = (((uint16_t)packet[pi + i] & 0x3D) << 8) | packet[pi];
    }

    return dp;
}

void sendData(DecodedPacket const &p)
{
    Serial.print(p.angle);
    Serial.print(" ");
    Serial.print(p.speed);
    Serial.print(" ");
    for (int i = 0; i < DISTANCES_PER_PACKET; i++) 
    {
        Serial.print(p.distance[i]);
        Serial.print(" ");
    }
    Serial.println();
}

void loop()
{
    static bool waitPacket = true;

    if (Serial1.available())
    {
        byte byteRead = Serial1.read();

        if (waitPacket)
        {
            if (byteRead == HEAD_BYTE)
            {
                packetIndex = 0;
                packet[packetIndex++] = byteRead;
                waitPacket = false;
            }
        }
        else
        {
            if (packet[0] == HEAD_BYTE)
            {
                packet[packetIndex++] = byteRead;

                if (packetIndex >= PACKET_SIZE)
                {
                    waitPacket = true;

                    DecodedPacket p = decodePacket();
                    sendData(p);
                }
            }
        }
    }
}