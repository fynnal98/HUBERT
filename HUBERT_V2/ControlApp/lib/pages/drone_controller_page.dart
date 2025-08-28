import 'dart:async';
import 'package:flutter/material.dart';
import '../widgets/joystick_widget.dart';
import '../services/udp_service.dart';

class DroneControllerPage extends StatefulWidget {
  const DroneControllerPage({Key? key}) : super(key: key);

  @override
  _DroneControllerPageState createState() => _DroneControllerPageState();
}

class _DroneControllerPageState extends State<DroneControllerPage> {
  double throttle = 0.0;
  double yaw = 0.0;
  double pitch = 0.0;
  double roll = 0.0;
  late UDPService udpService;
  Timer? _sendTimer;

  @override
  void initState() {
    super.initState();
    udpService = UDPService();
    // Sende die Steuerdaten alle 50ms
    _sendTimer = Timer.periodic(const Duration(milliseconds: 50), (timer) {
      udpService.sendControlData(throttle, yaw, pitch, roll);
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Controller'),
      ),
      body: Row(
        mainAxisAlignment: MainAxisAlignment.spaceEvenly,
        children: [
          CustomJoystick(
            title: "Linker Joystick",
            listener: (details) {
              setState(() {
                throttle = details.y;
                yaw = details.x;
              });
            },
          ),
          CustomJoystick(
            title: "Rechter Joystick",
            listener: (details) {
              setState(() {
                pitch = details.y;
                roll = details.x;
              });
            },
          ),
        ],
      ),
    );
  }

  @override
  void dispose() {
    _sendTimer?.cancel();
    super.dispose();
  }
}
