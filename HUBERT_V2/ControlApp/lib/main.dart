import 'package:flutter/material.dart';
import 'pages/settings_page.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Drone Controller',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: const SettingsPage(),
    );
  }
}
