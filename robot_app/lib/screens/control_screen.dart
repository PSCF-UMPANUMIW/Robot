import 'package:flutter/material.dart';
import 'package:robot_app/services/http_manager.dart';

class ControlScreen extends StatefulWidget {
  const ControlScreen({super.key});

  @override
  _ControlScreenState createState() => _ControlScreenState();
}

class _ControlScreenState extends State<ControlScreen> with AutomaticKeepAliveClientMixin {

  @override
  bool get wantKeepAlive => true;

  final HTTPManager _httpManager = HTTPManager();

  // Distance options
  final List<String> _distanceOptions = ['5cm', '20cm', '1m'];
  final List<double> _distanceValues = [50.0, 200.0, 1000.0]; // in mm
  int _selectedDistanceIndex = 0;

  // Angle options
  final List<String> _angleOptions = ['5deg', '1/8 turn', '1/4 turn'];
  final List<double> _angleValues = [87.27, 785.4, 1571.0]; // in milliradians
  int _selectedAngleIndex = 0;

  bool _isMoving = false;

  Future<void> _sendMoveCommand(String direction) async {
    setState(() {
      _isMoving = true;
    });

    try {
      double distance = _distanceValues[_selectedDistanceIndex];
      // If moving backward, negate the value
      if (direction == 'down') {
        distance = -distance;
      }

      // Updated to use "distance" key instead of "value"
      await _httpManager.post('move', {'distance': distance});
    } catch (e) {
      // Show error message
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(
          content: Text('Failed to send move command: ${e.toString()}'),
          backgroundColor: Colors.red,
        ),
      );
    } finally {
      if (mounted) {
        setState(() {
          _isMoving = false;
        });
      }
    }
  }

  Future<void> _sendTurnCommand(String direction) async {
    setState(() {
      _isMoving = true;
    });

    try {
      double angle = _angleValues[_selectedAngleIndex];
      // If turning left, negate the value
      if (direction == 'left') {
        angle = -angle;
      }

      // Updated to use "angle" key instead of "value"
      await _httpManager.post('turn', {'angle': angle});
    } catch (e) {
      // Show error message
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(
          content: Text('Failed to send turn command: ${e.toString()}'),
          backgroundColor: Colors.red,
        ),
      );
    } finally {
      if (mounted) {
        setState(() {
          _isMoving = false;
        });
      }
    }
  }

  Widget _buildSelectButtons({
    required String title,
    required List<String> options,
    required int selectedIndex,
    required Function(int) onSelected,
  }) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Text(
          title,
          style: const TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
        ),
        const SizedBox(height: 10),
        Row(
          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
          children: List.generate(options.length, (index) {
            final isSelected = selectedIndex == index;
            return Expanded(
              child: Padding(
                padding: const EdgeInsets.symmetric(horizontal: 4.0),
                child: ElevatedButton(
                  style: ElevatedButton.styleFrom(
                    backgroundColor: isSelected ? Colors.blue : Colors.grey.shade200,
                    foregroundColor: isSelected ? Colors.white : Colors.black87,
                  ),
                  onPressed: () {
                    onSelected(index);
                  },
                  child: Text(options[index]),
                ),
              ),
            );
          }),
        ),
      ],
    );
  }

  @override
  Widget build(BuildContext context) {
    super.build(context);
    return Scaffold(
      body: Center(
        child: Padding(
          padding: const EdgeInsets.symmetric(horizontal: 20.0),
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              // Distance Select Buttons
              _buildSelectButtons(
                title: 'Distance',
                options: _distanceOptions,
                selectedIndex: _selectedDistanceIndex,
                onSelected: (index) {
                  setState(() {
                    _selectedDistanceIndex = index;
                  });
                },
              ),
              const SizedBox(height: 40),

              // D-Pad
              _buildDPad(),
              const SizedBox(height: 40),

              // Angle Select Buttons
              _buildSelectButtons(
                title: 'Angle',
                options: _angleOptions,
                selectedIndex: _selectedAngleIndex,
                onSelected: (index) {
                  setState(() {
                    _selectedAngleIndex = index;
                  });
                },
              ),
            ],
          ),
        ),
      ),
    );
  }

  Widget _buildDPad() {
    const double buttonSize = 80.0;
    const double padding = 5.0;

    return AbsorbPointer(
      absorbing: _isMoving,
      child: Stack(
        alignment: Alignment.center,
        children: [
          Container(
            width: buttonSize * 3,
            height: buttonSize * 3,
            decoration: BoxDecoration(
              color: Colors.grey.shade100,
              borderRadius: BorderRadius.circular(16),
            ),
          ),
          // Up button
          Positioned(
            top: padding,
            child: _buildDirectionButton(
              icon: Icons.arrow_upward,
              label: 'Forward',
              onPressed: () => _sendMoveCommand('up'),
            ),
          ),
          // Down button
          Positioned(
            bottom: padding,
            child: _buildDirectionButton(
              icon: Icons.arrow_downward,
              label: 'Backward',
              onPressed: () => _sendMoveCommand('down'),
            ),
          ),
          // Left button
          Positioned(
            left: padding,
            child: _buildDirectionButton(
              icon: Icons.arrow_back,
              label: 'Left',
              onPressed: () => _sendTurnCommand('left'),
            ),
          ),
          // Right button
          Positioned(
            right: padding,
            child: _buildDirectionButton(
              icon: Icons.arrow_forward,
              label: 'Right',
              onPressed: () => _sendTurnCommand('right'),
            ),
          ),
          // Center indicator
          if (_isMoving)
            Container(
              width: buttonSize - 20,
              height: buttonSize - 20,
              decoration: BoxDecoration(
                color: Colors.grey,
                shape: BoxShape.circle,
              ),
              child: const Center(
                child: CircularProgressIndicator(),
              ),
            ),
        ],
      ),
    );
  }

  Widget _buildDirectionButton({
    required IconData icon,
    required String label,
    required VoidCallback onPressed,
  }) {
    return SizedBox(
      width: 80,
      height: 80,
      child: Column(
        mainAxisSize: MainAxisSize.min,
        children: [
          Expanded(
            child: ElevatedButton(
              style: ElevatedButton.styleFrom(
                shape: const CircleBorder(),
                padding: EdgeInsets.zero,
              ),
              onPressed: onPressed,
              child: Icon(
                icon,
                size: 32,
              ),
            ),
          ),
          const SizedBox(height: 4),
          Text(
            label,
            style: const TextStyle(fontSize: 12),
          ),
        ],
      ),
    );
  }
}