import 'package:flutter/material.dart';
import 'package:robot_app/services/http_manager.dart';

class ServerScreen extends StatefulWidget {
  const ServerScreen({super.key});

  @override
  _ServerScreenState createState() => _ServerScreenState();
}

class _ServerScreenState extends State<ServerScreen> with AutomaticKeepAliveClientMixin {
  @override
  bool get wantKeepAlive => true;

  final _httpManager = HTTPManager();
  final _serverIPController = TextEditingController();
  bool _isConnecting = false;

  // Store selected levels for each section
  int _selectedSpeed = 0;
  int _selectedAcceleration = 0;
  int _selectedCurrent = 0;
  int _selectedLidar = 0;

  // For button loading state
  int? _loadingSection; // 0: speed, 1: accel, 2: current, 3: lidar

  @override
  void initState() {
    super.initState();
    _serverIPController.text = "localhost";
  }

  @override
  void dispose() {
    _serverIPController.dispose();
    super.dispose();
  }

  Future<void> _updateServerIP() async {
    setState(() {
      _isConnecting = true;
    });

    try {
      await _httpManager.setServerIP(_serverIPController.text);
      await _httpManager.get('health');
      _showToast('Connected successfully!', success: true);
    } catch (e) {
      _showToast('Connection failed: ${e.toString()}', success: false);
    } finally {
      setState(() {
        _isConnecting = false;
      });
    }
  }

  Future<void> _sendLevel(String endpoint, int level, int sectionIndex) async {
    setState(() {
      _loadingSection = sectionIndex;
    });

    try {
      await _httpManager.post(endpoint, {"level": level});
      _showToast('Set $endpoint to $level', success: true);
    } catch (e) {
      _showToast('Failed to set $endpoint: $e', success: false);
    } finally {
      setState(() {
        _loadingSection = null;
      });
    }
  }

  void _showToast(String message, {required bool success}) {
    final color = success ? Colors.green : Colors.red;
    ScaffoldMessenger.of(context).showSnackBar(
      SnackBar(
        content: Text(message),
        backgroundColor: color,
        behavior: SnackBarBehavior.floating,
        duration: const Duration(seconds: 1),
      ),
    );
  }

  Widget _buildLevelSection({
    required String title,
    required List<String> labels,
    required int selected,
    required int maxLevel,
    required Function(int) onSelected,
    required String endpoint,
    required int sectionIndex,
  }) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Text(title, style: const TextStyle(fontWeight: FontWeight.bold, fontSize: 16)),
        const SizedBox(height: 8),
        Row(
          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
          children: List.generate(labels.length, (i) {
            final isSelected = selected == i;
            final isLoading = _loadingSection == sectionIndex && isSelected;
            return Expanded(
              child: Padding(
                padding: const EdgeInsets.symmetric(horizontal: 4.0),
                child: ElevatedButton(
                  style: ElevatedButton.styleFrom(
                    backgroundColor: isSelected ? Colors.blue : Colors.grey.shade200,
                    foregroundColor: isSelected ? Colors.white : Colors.black,
                  ),
                  onPressed: isLoading
                      ? null
                      : () {
                    onSelected(i);
                    _sendLevel(endpoint, i, sectionIndex);
                  },
                  child: isLoading
                      ? const SizedBox(
                    width: 18,
                    height: 18,
                    child: CircularProgressIndicator(strokeWidth: 2),
                  )
                      : Text(labels[i]),
                ),
              ),
            );
          }),
        ),
        const SizedBox(height: 16),
      ],
    );
  }

  @override
  Widget build(BuildContext context) {
    super.build(context);
    return Scaffold(
      appBar: AppBar(title: const Text('Settings')),
      body: SingleChildScrollView(
        child: Padding(
          padding: const EdgeInsets.all(16.0),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.stretch,
            children: [
              // Server address section
              Text("Server Address", style: const TextStyle(fontWeight: FontWeight.bold, fontSize: 16)),
              const SizedBox(height: 8),
              TextField(
                controller: _serverIPController,
                decoration: const InputDecoration(
                  labelText: 'Server IP Address'
                ),
              ),
              const SizedBox(height: 12),
              ElevatedButton(
                style: ElevatedButton.styleFrom(
                  backgroundColor: Colors.grey.shade200,
                  foregroundColor: Colors.black,
                ),
                onPressed: _isConnecting ? null : _updateServerIP,
                child: _isConnecting
                    ? const SizedBox(
                  height: 20,
                  width: 20,
                  child: CircularProgressIndicator(strokeWidth: 2),
                )
                    : const Text('Connect'),
              ),
              const SizedBox(height: 30),

              // Speed section
              _buildLevelSection(
                title: "Speed",
                labels: const ["Low", "Medium", "High"],
                selected: _selectedSpeed,
                maxLevel: 2,
                onSelected: (i) => setState(() => _selectedSpeed = i),
                endpoint: "speed",
                sectionIndex: 0,
              ),

              // Acceleration section
              _buildLevelSection(
                title: "Acceleration",
                labels: const ["Low", "Medium", "High"],
                selected: _selectedAcceleration,
                maxLevel: 2,
                onSelected: (i) => setState(() => _selectedAcceleration = i),
                endpoint: "acceleration",
                sectionIndex: 1,
              ),

              // Motor current section
              _buildLevelSection(
                title: "Motor Current",
                labels: const ["Low", "Medium", "High"],
                selected: _selectedCurrent,
                maxLevel: 2,
                onSelected: (i) => setState(() => _selectedCurrent = i),
                endpoint: "current",
                sectionIndex: 2,
              ),

              // LiDAR speed section
              _buildLevelSection(
                title: "LiDAR Speed",
                labels: const ["Off", "Slow", "Fast", "Max"],
                selected: _selectedLidar,
                maxLevel: 3,
                onSelected: (i) => setState(() => _selectedLidar = i),
                endpoint: "lidar",
                sectionIndex: 3,
              ),
            ],
          ),
        ),
      ),
    );
  }
}