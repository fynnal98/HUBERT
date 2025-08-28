import 'dart:typed_data';
import 'dart:math';
import 'package:udp/udp.dart';
import 'dart:io';

class UDPService {
  final String espIp = "192.168.4.1";
  final int espPort = 4210;
  late UDP sender;
  int messageCounter = 0;

  UDPService() {
    _initUDP();
  }

  Future<void> _initUDP() async {
    sender = await UDP.bind(Endpoint.any());
  }

  Future<void> sendControlData(double throttle, double yaw, double pitch, double roll) async {
    final List<int> message = [0x20];  // Startbyte
    message.add(0x10);  // Nachrichtentyp
    message.add(messageCounter);
    messageCounter = (messageCounter + 1) % 256;

    final List<int> payload = _encodePayload([throttle, yaw, pitch, roll]);
    message.add(payload.length);
    message.addAll(payload);

    int checksum = _calculateChecksum(message);
    message.add(checksum);
    message.add(0x30);  // Endbyte

    await sender.send(Uint8List.fromList(message), Endpoint.unicast(InternetAddress(espIp), port: Port(espPort)));
  }

  Future<void> sendParameter(String path, String value) async {
    final String message = "$path=$value";
    await sender.send(message.codeUnits, Endpoint.unicast(InternetAddress(espIp), port: Port(espPort)));
  }

  Future<void> sendSaveCommand() async {
    await sender.send("save".codeUnits, Endpoint.unicast(InternetAddress(espIp), port: Port(espPort)));
  }

  int _calculateChecksum(List<int> message) {
    return message.reduce((a, b) => a + b) % 256;
  }

  List<int> _encodePayload(List<double> values) {
    final List<int> payload = [];
    for (double value in values) {
      int sbusValue = ((value + 1) * 1023).clamp(0, 2047).toInt();
      payload.add(sbusValue & 0xFF);
      payload.add((sbusValue >> 8) & 0xFF);
    }
    return payload;
  }
}
