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

    // Display sensor data
    return Padding(
      padding: const EdgeInsets.all(16.0),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          _buildSensorCard(
            title: 'Orientation',
            icon: Icons.rotate_90_degrees_ccw,
            values: [
              SensorValue(
                name: 'Pitch',
                value: '${_sensorData?.pitch.toStringAsFixed(2)}°',
              ),
              SensorValue(
                name: 'Roll',
                value: '${_sensorData?.roll.toStringAsFixed(2)}°',
              ),
            ],
          ),
          const SizedBox(height: 16),
          _buildSensorCard(
            title: 'Position',
            icon: Icons.gps_fixed,
            values: [
              SensorValue(
                name: 'X',
                value: '${_sensorData?.posX.toStringAsFixed(2)}',
              ),
              SensorValue(
                name: 'Y',
                value: '${_sensorData?.posY.toStringAsFixed(2)}',
              ),
            ],
          ),
          const SizedBox(height: 16),
          _buildSensorCard(
            title: 'Environment',
            icon: Icons.thermostat,
            values: [
              SensorValue(
                name: 'Temperature',
                value: '${_sensorData?.temperature.toStringAsFixed(1)}°C',
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