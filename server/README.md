# Robot Server

A simple Flask server that forms a bridge between the robot and the client application. It communicates with the client's application via a REST API, and with the robot via ESP-NOW through an ESP32 connected to the server by a serial port. See the hardware diagram in the [repository's root](../README.md) for more details.

## Setup

1. Install the required packages:
```
pip install -r requirements.txt
```

2. Run the server:
```
python app.py
```

The server will start on port 5000.

## Available Endpoints

### GET /health

- **Description:** Health check endpoint. Used by the client application to check there is a running server at the specified address.
- **Response:**  
  ```json
  {
    "status": "healthy",
    "message": "Robot Server is running"
  }
  ```

---

### GET /sensors

- **Description:** Returns the latest sensor data from the robot.
- **Response:**  
  ```json
  {
    "ground_0": 0.82,
    "ground_1": 0.89,
    "ground_2": 0.86,
    "fronnt_dist": 0.34,
    "batt_v": 15.82,
    "roll": 2.1,
    "pitch": -1.2,
    "heading": 36.5,
    "lidar_speed": 182.4,
  }
  ```

---

### POST /turn

- **Description:** Rotates the robot by a specified angle.
- **Request Body:**  
  ```json
  {
    "angle": 90
  }
  ```
- **Response:**  
  ```json
  {
    "status": "success",
    "message": "Turn command with angle 90 received"
  }
  ```

---

### POST /move

- **Description:** Moves the robot by a specified distance.
- **Request Body:**  
  ```json
  {
    "distance": 100
  }
  ```
- **Response:**  
  ```json
  {
    "status": "success",
    "message": "Move command with distance 100 received"
  }
  ```

---

### POST /speed

- **Description:** Sets the robot's speed level. Allowed values are 0 (low), 1 (medium) and 2 (high).
- **Request Body:**  
  ```json
  {
    "level": 2
  }
  ```
- **Response:**  
  ```json
  {
    "status": "success",
    "message": "Speed command with speed 2 received"
  }
  ```

---

### POST /acceleration

- **Description:** Sets the robot's acceleration level. Allowed values are 0 (low), 1 (medium) and 2 (high).
- **Request Body:**  
  ```json
  {
    "level": 1
  }
  ```
- **Response:**  
  ```json
  {
    "status": "success",
    "message": "Acceleration command with level 1 received"
  }
  ```

---

### POST /current

- **Description:** Sets the robot's current limit. Allowed values are 0 (low), 1 (medium) and 2 (high).
- **Request Body:**  
  ```json
  {
    "level": 0
  }
  ```
- **Response:**  
  ```json
  {
    "status": "success",
    "message": "Current command with level 0 received"
  }
  ```

---

### POST /lidar

- **Description:** Sets the LIDAR speed level. Allowed values are 0 (off), 1 (slow), 2 (fast) and 3 (max).
> [!WARNING]
> The lidar is incabaple of measurement in the "max" mode.
- **Request Body:**  
  ```json
  {
    "level": 3
  }
  ```
- **Response:**  
  ```json
  {
    "status": "success",
    "message": "Lidar command with level 3 received"
  }
  ```

---

## Examples

### Using curl

Get sensor data:
```
curl http://localhost:5000/sensors
```

Send movement command:
```
curl -X POST http://localhost:5000/move -H "Content-Type: application/json" -d "{\\"type\\": \\"rotate\\", \\"value\\": 45}"
```
