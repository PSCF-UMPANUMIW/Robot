# Robot firmware

This directory contains the firmware for the ESP32 microcontroller that controls the robot. The firmware is written in C++ and uses [PlatformIO](https://github.com/platformio) as its build system together with the [Arduino](https://github.com/arduino) framework.

## Project structure

The project is structured using the typical PlatformIO layout:
- `src/` - contains the source code of the firmware
- `include/` - contains header files
- `lib/` - contains libraries used by the firmware
- `platformio.ini` - the configuration file for PlatformIO, specifying the board type, libraries, and build options

The project uses the PlatformIO feature that allows multiple hardware configurations to use different source files. This allowed us to create a single project that can be built for both the robot's onboard ESP32 as well as the ESP32 connected to the server, which is used for communication with the robot via ESP-NOW. This approach simplifies the development process as it allows for easy sharing of code between the two configurations.

## Building and uploading the firmware

To build and upload the firmware to the ESP32, follow these steps:

1. Set up [PlatformIO](https://platformio.org/install) if you haven't already.

1. Open the project in your preferred IDE (e.g., VSCode with PlatformIO extension).

1. Make sure the `platformio.ini` file is correctly configured for your ESP32 boards. There is a separate configuration for the robot's ESP32 and the base station's ESP32. The default configuration should work for most ESP32 boards, but you can adjust it if necessary. You may need to specify the upload port in the `platformio.ini` file, for example:
   ```ini
   upload_port = /dev/ttyUSB0  # or COM3 on Windows
   ```
   Make sure to replace `/dev/ttyUSB0` with the actual port your ESP32 is connected to. Alternatively, you can remove this line entirely, and PlatformIO will try to automatically detect the port when you connect the ESP32.

1. Locate the file `include/constants.h` and fill in the MAC addresses of the ESP's you're using. The file contains constants:
   - `ROBOT_MAC_ADDR` - the MAC address of the robot's ESP32
   - `BASE_STATION_MAC_ADDR` - the MAC address of the base station's ESP32

    Sample values are provided in the file, but you should replace them with the actual MAC addresses of your ESP32 devices. You can find the MAC address by running the following code on your ESP32:
   ```cpp
   Serial.println(WiFi.macAddress());
   ```

1. Select the appropriate environment in PlatformIO (e.g., `esp32_robot` for the robot's ESP32 or `esp32_base_station` for the server's ESP32).

1. Build the project by clicking the "Build" button in the PlatformIO toolbar or running `pio run` in the terminal.

1. Upload the firmware to the ESP32 by clicking the "Upload" button in the PlatformIO toolbar or running `pio run --target upload` in the terminal.

1. Repeat for the other ESP32 configuration if necessary.


## External dependencies

The project uses the following external libraries, which are specified in the `platformio.ini` file:

- [TMCStepper](https://github.com/teemuatlut/TMCStepper) - handles the communication with the TMC2209 stepper motor driver via UART for setting up the driver's parameters (phase current, microstepping, etc.) and for reading the driver's status.
- [AccelStepper](https://github.com/waspinator/AccelStepper) - generates step/direction signals for the stepper motors, and interpolates the speed allowing for smooth and controlled acceleration and deceleration.

## Internal libraries

For the project, the following internal libraries were created:

- **XiaomiLidarReader** - reads and decodes data from the Xiaomi vacuum cleaner LIDAR device.
- **WheelPlatform** - manages stepper motors and provides a simple interface for moving the robot while abstracting away the kinematics of the platform.
- **Sensors** - provides an interface for reading various sensors, such as the ultrasonic or IR distance, IMU, and magnetometer sensor, and allows for easy access to their data through the SensorManager class.
- **EspNowClient** - sends and receives messages via ESP-NOW, wile automatically handling the serialization and deserialization of messages using C++'s template system.

---

Go back to the main project [README](../README.md) for other details.