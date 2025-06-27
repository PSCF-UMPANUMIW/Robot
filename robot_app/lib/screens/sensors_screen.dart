import 'dart:async';
import 'package:flutter/material.dart';
import 'package:robot_app/services/http_manager.dart';
import 'package:robot_app/models/sensor_data.dart';

class SensorsScreen extends StatefulWidget {
  const SensorsScreen({super.key});

  @override
  _SensorsScreenState createState() => _SensorsScreenState();
}

class _SensorsScreenState extends State<SensorsScreen> {
  final HTTPManager _httpManager = HTTPManager();
  Timer? _timer;
  SensorData? _sensorData;
  bool _isLoading = true;
  String _error = '';

  @override
  void initState() {
    super.initState();
    // Start fetching data immediately
    _fetchSensorData();

    _timer = Timer.periodic(const Duration(milliseconds: 1000), (timer) {
      _fetchSensorData();
    });
  }

  @override
  void dispose() {
    // Cancel the timer when the widget is disposed
    _timer?.cancel();
    super.dispose();
  }

  Future<void> _fetchSensorData() async {
    try {
      final response = await _httpManager.get('sensors');

      setState(() {
        _sensorData = SensorData.fromJson(response);
        _isLoading = false;
        _error = '';
      });
    } catch (e) {
      setState(() {
        _error = 'Failed to fetch sensor data: ${e.toString()}';
        _isLoading = false;
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Sensors'),
      ),
      body: _buildBody(),
    );
  }

  Widget _buildBody() {
    if (_isLoading && _sensorData == null) {
      return const Center(child: CircularProgressIndicator());
    }

    if (_error.isNotEmpty && _sensorData == null) {
      return Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Icon(Icons.error_outline, color: Colors.red, size: 60),
            const SizedBox(height: 16),
            Text(
              _error,
              style: const TextStyle(color: Colors.red),
              textAlign: TextAlign.center,
            ),
            const SizedBox(height: 16),
            ElevatedButton(
              onPressed: _fetchSensorData,
              child: const Text('Retry'),
            ),
          ],
        ),
      );
    }

    return SingleChildScrollView(
      padding: const EdgeInsets.all(16.0),
      child: _sensorData == null
          ? const SizedBox.shrink()
          : Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          _buildSensorCard(
            title: 'Ground Sensors',
            icon: Icons.linear_scale,
            values: [
              SensorValue(
                name: 'Left',
                value: _sensorData!.groundSensorL.toStringAsFixed(2),
              ),
              SensorValue(
                name: 'Front',
                value: _sensorData!.groundSensorF.toStringAsFixed(2),
              ),
              SensorValue(
                name: 'Right',
                value: _sensorData!.groundSensorR.toStringAsFixed(2),
              ),
            ],
          ),
          const SizedBox(height: 16),
          _buildSensorCard(
            title: 'Front Sensor',
            icon: Icons.sensors,
            values: [
              SensorValue(
                name: 'Front Sensor',
                value: _sensorData!.frontSensor.toStringAsFixed(2),
              ),
            ],
          ),
          const SizedBox(height: 16),
          _buildSensorCard(
            title: 'Battery',
            icon: Icons.battery_full,
            values: [
              SensorValue(
                name: 'Voltage',
                value: '${_sensorData!.batteryVoltage.toStringAsFixed(2)} V',
              ),
            ],
          ),
          const SizedBox(height: 16),
          _buildSensorCard(
            title: 'Orientation',
            icon: Icons.rotate_90_degrees_ccw,
            values: [
              SensorValue(
                name: 'Pitch',
                value: '${_sensorData!.pitch.toStringAsFixed(2)}°',
              ),
              SensorValue(
                name: 'Roll',
                value: '${_sensorData!.roll.toStringAsFixed(2)}°',
              ),
              SensorValue(
                name: 'Heading',
                value: '${_sensorData!.heading.toStringAsFixed(2)}°',
              ),
            ],
          ),
          const SizedBox(height: 16),
          _buildSensorCard(
            title: 'LiDAR',
            icon: Icons.speed,
            values: [
              SensorValue(
                name: 'Speed',
                value: '${_sensorData!.lidarSpeed.toStringAsFixed(2)}',
              ),
            ],
          ),
        ],
      ),
    );
  }

  Widget _buildSensorCard({
    required String title,
    required IconData icon,
    required List<SensorValue> values,
  }) {
    return Card(
      elevation: 4,
      child: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Row(
              children: [
                Icon(icon, size: 28),
                const SizedBox(width: 12),
                Text(
                  title,
                  style: const TextStyle(
                    fontSize: 20,
                    fontWeight: FontWeight.bold,
                  ),
                ),
              ],
            ),
            const Divider(height: 24),
            ...values.map((value) => Padding(
              padding: const EdgeInsets.symmetric(vertical: 4.0),
              child: Row(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: [
                  Text(
                    value.name,
                    style: const TextStyle(
                      fontSize: 16,
                      fontWeight: FontWeight.w500,
                    ),
                  ),
                  Text(
                    value.value,
                    style: const TextStyle(
                      fontSize: 16,
                      fontFamily: 'monospace',
                    ),
                  ),
                ],
              ),
            )),
          ],
        ),
      ),
    );
  }
}

class SensorValue {
  final String name;
  final String value;

  SensorValue({required this.name, required this.value});
}