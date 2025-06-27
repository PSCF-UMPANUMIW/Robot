> [!CAUTION]
> This README is still work in progress

# Universal Robotic Platform

## Description

###  PL
Projekt na przedmioty *Projektowanie Systemów Cybernetyczno-Fizycznych* oraz *Uniwersalne Metody Projektowania Aplikacji na Urządzenia Mobilne i Wbudowane* w ramach 6. semestru studiów kierunku Informatyka na wydziale Automatyki, Elektroniki i Informatyki **Politechniki Śląskiej**.

### EN
Project for two courses during the 6th semester of the Informatics major on the **Silesian University of Technology's** Faculty of Automatic Control, Electronics and Computer Science.

---

## Goals

The focus of this project is to design and build a modular AGV (Automated Guided Vehicle) platform that can carry out various tasks indoors, depending on the installed functional module. Key objectives include:

- Creating a robotic base with a defined mounting area for interchangeable modules (e.g., vacuum cleaner, surface scanner, service robot).
- Enabling in-place rotation around the geometric center without changing the occupied footprint ("round chassis").
- Achieving precise positioning:
  - Linear accuracy ≤ ±10 cm in each axis
  - Orientation accuracy ≤ ±3°
- Obstacle detection and distance measurement:
  - Range of 10–60 cm
  - Measurement precision ≤ 1 cm
- Mapping and positioning with LiDAR
- Safe ground-edge detection (e.g., to avoid falling stairs) around the wheels. 
- Maximum forward speed up to 0.8 m/s and a full rotation in ≤ 3 s.
- A fully modular mechanical design for easy replacement of all components.

---

## Project Structure

The project is organized into several modules, each serving a specific purpose:

- `ESP-Robot`
  PlatformIO project for the ESP32 robot and base station controller

- `LIDAR-Vis`
  Python-based visualization tool for rendering LiDAR scan data in real time.

- `server`
  A lightweight server for local API

- `robot_app`  
  Cross-platform application for client device

For detailed documentation on each module, please refer to the respective README.md files in the `/ESP-Robot`, `/LIDAR-Vis`, `/server` and `/robot_app` directories.

---

## Results

![Hardware diagram](images/hardware_diagram.jpg)

![Robot photo 1](images/robot_photo.png)

![Robot LIDAR showcase](images/robot_lidar.png)

![App main screen](images/app.png)

---

## Features

- beep
- boop

---

## Technologies involved

The project uses the following technologies:

- **C++** - for programming the ESP32 microcontrollers
- [**PlatformIO**](https://github.com/platformio) - build system for the microcontrollers with the [Arduino](https://github.com/arduino) framework
- **Flutter and Dart** - for creating the cross-platform GUI application,
- **Python** - for lidar data visualization as well as hosting the "base station" server

---

## Authors

- [Karol Orszulik](https://github.com/KarolOrszulik)
- [Karol Gryc](https://github.com/KarolGryc)
- [Krzysztof Witek](https://github.com/krzsztfwtk)

---

## License

This project is licensed under GNU General Public License v3.0. See [LICENSE](LICENSE) for details.
