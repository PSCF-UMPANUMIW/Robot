class SensorData {
  final double pitch;
  final double roll;
  final double posX;
  final double posY;
  final double temperature;

  SensorData({
    required this.pitch,
    required this.roll,
    required this.posX,
    required this.posY,
    required this.temperature,
  });

  factory SensorData.fromJson(Map<String, dynamic> json) {
    return SensorData(
      pitch: json['pitch']?.toDouble() ?? 0.0,
      roll: json['roll']?.toDouble() ?? 0.0,
      posX: json['posX']?.toDouble() ?? 0.0,
      posY: json['posY']?.toDouble() ?? 0.0,
      temperature: json['temperature']?.toDouble() ?? 0.0,
    );
  }
}