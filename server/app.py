from flask import Flask, request, jsonify
from flask_cors import CORS
import serial
import threading

SERIAL_PORT = 'COM11'
SERIAL_BAUDRATE = 115200 

app = Flask(__name__)
CORS(app)

esp = None

sensors_data = {}

@app.route('/health', methods=['GET'])
def health():
    return jsonify({"status": "healthy", "message": "Robot Mock Server is running"}), 200


@app.route('/sensors', methods=['GET'])
def sensors():
    return jsonify(sensors_data), 200


@app.route('/turn', methods=['POST'])
def turn():
    data = request.get_json()
    
    if not data:
        return jsonify({"error": "Invalid request data"}), 400
    
    angle = data.get('angle')
    
    if angle is None:
        return jsonify({"error": "Missing angle parameter"}), 400
    
    print(f"Received turn command with angle: {angle}")
    esp.write(f"R{angle}\n".encode('utf-8'))
    
    return jsonify({"status": "success", "message": f"Turn command with angle {angle} received"}), 200


@app.route('/move', methods=['POST'])
def move():
    data = request.get_json()
    
    if not data:
        return jsonify({"error": "Invalid request data"}), 400
    
    distance = data.get('distance')

    if distance is None:
        return jsonify({"error": "Missing distance parameter"}), 400
    
    print(f"Received move command with distance: {distance}")
    esp.write(f"M{distance}\n".encode('utf-8'))
    
    return jsonify({"status": "success", "message": f"Move command with distance {distance} received"}), 200


@app.route('/speed', methods=['POST'])
def speed():
    data = request.get_json()
    
    if not data:
        return jsonify({"error": "Invalid request data"}), 400
    
    speedLevel = int(data.get('level'))

    if speedLevel is None:
        return jsonify({"error": "Missing speed parameter"}), 400
    
    print(f"Received speed command with speed: {speedLevel}")
    esp.write(f"s{speedLevel}\n".encode('utf-8'))
    
    return jsonify({"status": "success", "message": f"Speed command with speed {speedLevel} received"}), 200


@app.route('/acceleration', methods=['POST'])
def acceleration():
    data = request.get_json()
    
    if not data:
        return jsonify({"error": "Invalid request data"}), 400
    
    accelerationLevel = int(data.get('level'))

    if accelerationLevel is None:
        return jsonify({"error": "Missing acceleration parameter"}), 400
    
    print(f"Received acceleration command with level: {accelerationLevel}")
    esp.write(f"a{accelerationLevel}\n".encode('utf-8'))
    
    return jsonify({"status": "success", "message": f"Acceleration command with level {accelerationLevel} received"}), 200


@app.route('/current', methods=['POST'])
def current():
    data = request.get_json()
    
    if not data:
        return jsonify({"error": "Invalid request data"}), 400
    
    currentLevel = int(data.get('level'))

    if currentLevel is None:
        return jsonify({"error": "Missing current parameter"}), 400
    
    print(f"Received current command with level: {currentLevel}")
    esp.write(f"c{currentLevel}\n".encode('utf-8'))
    
    return jsonify({"status": "success", "message": f"Current command with level {currentLevel} received"}), 200


@app.route('/lidar', methods=['POST'])
def lidar():
    data = request.get_json()
    
    if not data:
        return jsonify({"error": "Invalid request data"}), 400
    
    lidarSpeed = int(data.get('level'))

    if lidarSpeed is None:
        return jsonify({"error": "Missing command parameter"}), 400
    
    print(f"Received lidar speed command with level: {lidarSpeed}")
    esp.write(f"L{lidarSpeed}\n".encode('utf-8'))
    
    return jsonify({"status": "success", "message": f"Lidar command with level {lidarSpeed} received"}), 200


def serial_receive_callback():
    while True:
        try:
            line = esp.readline().decode('utf-8').strip()
            if len(line) == 0:
                continue
            key, value = line.split(':', 1)
            key = key.strip()[1:]
            value = value.strip()
            sensors_data[key] = float(value)
        except serial.SerialException as e:
            print(f"Serial error: {e}")
        except UnicodeDecodeError:
            print("No newline character received or data not in UTF-8 format.")
        except Exception as e:
            print(f"Unexpected error: {e}")


def main():
    global esp
    try:
        esp = serial.Serial(SERIAL_PORT, SERIAL_BAUDRATE, timeout=1)
        print("Serial port initialized.")

        serial_thread = threading.Thread(target=serial_receive_callback)
        serial_thread.daemon = True  # allow the thread to exit when the main thread exits
        serial_thread.start()

    except serial.SerialException as e:
        print(f"Error initializing serial port: {e}")
        exit(1)

    app.run(host='0.0.0.0', port=5000, debug=False)


if __name__ == '__main__':
    main()
