from flask import Flask, request, jsonify
from flask_cors import CORS
import random

app = Flask(__name__)
CORS(app)

# Simulated sensor data
def get_sensor_data():
    return {
        "posX": round(random.uniform(-10, 10), 2),
        "posY": round(random.uniform(-10, 10), 2),
        "roll": round(random.uniform(-180, 180), 2),
        "pitch": round(random.uniform(-90, 90), 2),
        "temperature": round(random.uniform(20, 30), 2)
    }


@app.route('/sensors', methods=['GET'])
def sensors():
    # Return simulated sensor data
    return jsonify(get_sensor_data())


@app.route('/health', methods=['GET'])
def health():
    return jsonify({"status": "healthy", "message": "Robot Mock Server is running"}), 200


@app.route('/turn', methods=['POST'])
def turn():
    data = request.get_json()
    
    if not data:
        return jsonify({"error": "Invalid request data"}), 400
    
    angle = data.get('angle')
    
    if angle is None:
        return jsonify({"error": "Missing angle parameter"}), 400
    
    # Print to terminal as requested
    print(f"Received turn command with angle: {angle}")
    
    return jsonify({"status": "success", "message": f"Turn command with angle {angle} received"})


@app.route('/move', methods=['POST'])
def move():
    data = request.get_json()
    
    if not data:
        return jsonify({"error": "Invalid request data"}), 400
    
    distance = data.get('distance')
    
    # Print to terminal as requested
    print(f"Received move command with distance: {distance}")
    
    return jsonify({"status": "success", "message": f"Move command with distance {distance} received"})

if __name__ == '__main__':
    print("Starting Robot Mock Server...")
    app.run(host='0.0.0.0', port=5000, debug=True)
