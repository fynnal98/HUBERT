import 'package:flutter/material.dart';
import 'package:flutter_joystick/flutter_joystick.dart';

class CustomJoystick extends StatelessWidget {
  final String title;
  final Function(StickDragDetails) listener;

  const CustomJoystick({
    Key? key,
    required this.title,
    required this.listener,
  }) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Column(
      mainAxisAlignment: MainAxisAlignment.center,
      children: [
        Text(
          title,
          style: const TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
        ),
        const SizedBox(height: 10),
        SizedBox(
          width: 200,
          height: 200,
          child: Joystick(
            mode: JoystickMode.all,
            listener: listener,
            base: Container(
              decoration: BoxDecoration(
                shape: BoxShape.circle,
                border: Border.all(color: Colors.orange, width: 3.0),  // Orangefarbener Rahmen
                color: Colors.transparent,
              ),
            ),
            stick: Container(
              decoration: const BoxDecoration(
                shape: BoxShape.circle,
                color: Colors.black,  // Schwarzer Stick
              ),
              width: 50,
              height: 50,
            ),
          ),
        ),
      ],
    );
  }
}
