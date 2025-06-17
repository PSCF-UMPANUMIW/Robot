import 'package:flutter/material.dart';
import 'package:robot_app/services/http_manager.dart';

class ServerScreen extends StatefulWidget {
  const ServerScreen({super.key});

  @override
  _ServerScreenState createState() => _ServerScreenState();
}

class _ServerScreenState extends State<ServerScreen> {
  final _httpManager = HTTPManager();
  final _serverIPController = TextEditingController();
  bool _isConnecting = false;
  String _statusMessage = '';

  @override
  void initState() {
    super.initState();
    _serverIPController.text = _httpManager.serverIP;
  }

  @override
  void dispose() {
    _serverIPController.dispose();
    super.dispose();
  }

  Future<void> _updateServerIP() async {
    setState(() {
      _isConnecting = true;
      _statusMessage = '';
    });

    try {
      await _httpManager.setServerIP(_serverIPController.text);

      // Test the connection
      await _httpManager.get('health');

      setState(() {
        _statusMessage = 'Connected successfully!';
      });
    } catch (e) {
      setState(() {
        _statusMessage = 'Connection failed: ${e.toString()}';
      });
    } finally {
      setState(() {
        _isConnecting = false;
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('Server Configuration')),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.stretch,
          children: [
            TextField(
              controller: _serverIPController,
              decoration: const InputDecoration(
                labelText: 'Server IP Address',
                hintText: 'e.g., http://192.168.1.100:8080',
              ),
            ),
            const SizedBox(height: 24),
            ElevatedButton(
              onPressed: _isConnecting ? null : _updateServerIP,
              child: _isConnecting
                  ? const CircularProgressIndicator()
                  : const Text('Connect'),
            ),
            if (_statusMessage.isNotEmpty)
              Padding(
                padding: const EdgeInsets.only(top: 16.0),
                child: Text(
                  _statusMessage,
                  style: TextStyle(
                    color: _statusMessage.contains('failed')
                        ? Colors.red
                        : Colors.green,
                  ),
                ),
              ),
          ],
        ),
      ),
    );
  }
}