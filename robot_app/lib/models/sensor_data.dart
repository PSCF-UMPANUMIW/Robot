class SensorData {
  final double groundSensorL;
  final double groundSensorF;
  final double groundSensorR;
  final double frontSensor;
  final double batteryVoltage;
  final double pitch;
  final double roll;
  final double heading;
  final double lidarSpeed;

  SensorData({
    required this.groundSensorL,
    required this.groundSensorF,
    required this.groundSensorR,
    required this.frontSensor,
    required this.batteryVoltage,
    required this.pitch,
    required this.roll,
    required this.heading,
    required this.lidarSpeed,
  });

  factory SensorData.fromJson(Map<String, dynamic> json) {
    return SensorData(
      groundSensorL: (json['ground_1'] ?? 0.0).toDouble(),
      groundSensorF: (json['ground_2'] ?? 0.0).toDouble(),
      groundSensorR: (json['ground_0'] ?? 0.0).toDouble(),
      frontSensor: (json['front_dist'] ?? 0.0).toDouble(),
      batteryVoltage: (json['batt_v'] ?? 0.0).toDouble(),
      pitch: (json['pitch'] ?? 0.0).toDouble(),
      roll: (json['roll'] ?? 0.0).toDouble(),
      heading: (json['heading'] ?? 0.0).toDouble(),
      lidarSpeed: (json['lidar_speed'] / 10.0 ?? 0.0).toDouble(),
    );
  }
}