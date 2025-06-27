import 'dart:convert';
import 'package:http/http.dart' as http;
import 'package:shared_preferences/shared_preferences.dart';

class HTTPManager {
  static final HTTPManager _instance = HTTPManager._internal();

  static final int port = 5000;

  // Singleton pattern
  factory HTTPManager() {
    return _instance;
  }

  HTTPManager._internal();

  // Default server address
  String _serverIP = 'http://localhost:5000';

  // Get the current server IP
  String get serverIP => _serverIP;

  // Initialize the HTTP manager with saved server IP
  Future<void> initialize() async {
    final prefs = await SharedPreferences.getInstance();
    _serverIP = prefs.getString('server_ip') ?? _serverIP;
  }

  // Update the server IP address
  Future<void> setServerIP(String ip) async {
    _serverIP = ip.startsWith('http') ? ip : 'http://$ip:$port';
    final prefs = await SharedPreferences.getInstance();
    await prefs.setString('server_ip', _serverIP);
  }

  // GET request
  Future<Map<String, dynamic>> get(String endpoint) async {
    try {
      final url = Uri.parse('$_serverIP/$endpoint');
      final response = await http.get(url);

      if (response.statusCode == 200) {
        return jsonDecode(response.body);
      } else {
        throw Exception('Failed to load data: ${response.statusCode}');
      }
    } catch (e) {
      throw Exception('Network error: $e');
    }
  }

  // POST request
  Future<Map<String, dynamic>> post(String endpoint, Map<String, dynamic> data) async {
    try {
      final url = Uri.parse('$_serverIP/$endpoint');
      final response = await http.post(
        url,
        headers: {'Content-Type': 'application/json'},
        body: jsonEncode(data),
      );

      if (response.statusCode == 200 || response.statusCode == 201) {
        return jsonDecode(response.body);
      } else {
        throw Exception('Failed to post data: ${response.statusCode}');
      }
    } catch (e) {
      throw Exception('Network error: $e');
    }
  }
}