#include <Arduino.h>
#include <HardwareSerial.h>

#define LIDAR_TX 27

static bool acceptsInterrupts = true;
static byte buffer[1024];
static int size;

void printBuffer() {
    Serial.printf("(0x%02x) ", size);
    for (int i = 0; i < size; i++) {
        Serial.printf("%02x ", buffer[i]);
    }
    Serial.println();
}

void f(void*) {
    delay(12);

    size = Serial1.available();
    Serial1.readBytes(buffer, size);

    printBuffer();
    
    acceptsInterrupts = true;

    vTaskDelete(NULL);
}

void IRAM_ATTR isr() {
    if (!acceptsInterrupts)
        return;

    xTaskCreatePinnedToCore(f, NULL, 8192, NULL, 1, NULL, 0);

    acceptsInterrupts = false;
}

void setup() {
    Serial.begin(921600);
    Serial1.begin(115200, SERIAL_8N1, LIDAR_TX, -1);
     
    attachInterrupt(digitalPinToInterrupt(LIDAR_TX), isr, FALLING);
}


void loop() {
    delay(1000);
}