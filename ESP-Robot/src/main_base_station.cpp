#include <Arduino.h>

void setup()
{
    Serial.begin(115200);
    Serial.println("Base Station Setup Complete");
}

void loop()
{
    static int n = 0;
    Serial.printf("Base Station Loop %d\n", n++);

    if (digitalRead(0) == LOW)
    {
        Serial.println("Base Station button pressed");
        delay(500);
    }
    delay(1000);
}