import 'package:flutter/material.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';

void main() {
  runApp(MyApp(
  ));
}

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  MqttServerClient? client;
  final String topic = "Omkar_Guha_motor"; // Topic matching the NodeMCU code

  @override
  void initState() {
    super.initState();
    _connect();
  }

  Future<void> _connect() async {
    client = MqttServerClient('broker.hivemq.com', 'flutter_client_12345678'); // Unique client ID
    client!.port = 1883; // MQTT broker port
    client!.logging(on: true);
    client!.keepAlivePeriod = 20;
    client!.onDisconnected = _onDisconnected;

    try {
      await client!.connect();
      if (client!.connectionStatus!.state == MqttConnectionState.connected) {
        print('MQTT Connected');
      } else {
        print('ERROR: MQTT connection failed - disconnecting, state is ${client!.connectionStatus!.state}');
        _disconnect();
      }
    } catch (e) {
      print('Exception: $e');
      _disconnect();
    }
  }

  void _disconnect() {
    client?.disconnect();
    _onDisconnected();
  }

  void _onDisconnected() {
    print('MQTT Disconnected');
  }

  void _publishMessage(String message) {
    final builder = MqttClientPayloadBuilder();
    builder.addString(message);
    client!.publishMessage(topic, MqttQos.exactlyOnce, builder.payload!);
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: Text('MQTT LED Control'),
        ),
        body: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              ElevatedButton(
                onPressed: () {
                  _publishMessage('ON'); // Send 'ON' to turn the LED on
                },
                child: Text('Turn LED ON'),
              ),
              ElevatedButton(
                onPressed: () {
                  _publishMessage('OFF'); // Send 'OFF' to turn the LED off
                },
                child: Text('Turn LED OFF'),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
