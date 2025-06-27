# Robot Mock Server

A simple Flask server that simulates robot sensors and accepts movement commands.

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

### GET /sensors

Returns simulated sensor data in JSON format:
```json
{
  "posX": 5.23,
  "posY": -2.45,
  "roll": 10.5,
  "pitch": -5.2,
  "temperature": 25.3
}
```

### POST /move

Accepts movement commands in JSON format.

Request body:
```json
{
  "type": "rotate",  // or "drive"
  "value": 90.5      // can be any numeric value
}
```

Response:
```json
{
  "status": "success",
  "message": "rotate command with value 90.5 received"
}
```

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
