#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Replace with your network credentials
const char* ssid = "703";
const char* password = "703_boys";

// HiveMQ public broker details
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

// Replace with your unique client ID
const char* clientId = "nodeMCU_12345678_1";

// Publishing interval
const long publishInterval = 5000; // 5 seconds
unsigned long lastPublishTime = 0;

// Pin for the built-in LED (GPIO2, D4)
const int ledPin = D4; // GPIO2 (D4) on NodeMCU

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT); // Initialize the built-in LED pin as an output
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  if (client.connect(clientId)) {
    Serial.println("Connected to MQTT broker");
    client.subscribe("Omkar_Guha_motor"); // Update to your topic
  } else {
    Serial.print("Failed to connect, rc=");
    Serial.print(client.state());
    delay(2000);
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Publish a message at regular intervals
  unsigned long currentMillis = millis();
  if (currentMillis - lastPublishTime >= publishInterval) {
    lastPublishTime = currentMillis;
    client.publish("Omkar_Guha_motor", "Hello Motor_Guha"); // Update to your topic
  }
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientId)) {
      Serial.println("connected");
      client.subscribe("your/topic"); // Update to your topic
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  // Control the built-in LED based on the message
  if (message == "ON") {
    digitalWrite(ledPin, LOW); // Turn built-in LED on (LOW to turn it on)
  } else if (message == "OFF") {
    digitalWrite(ledPin, HIGH); // Turn built-in LED off (HIGH to turn it off)
  }
}
