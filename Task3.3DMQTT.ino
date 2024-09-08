#include <WiFiNINA.h>
#include <PubSubClient.h>

// Wi-Fi credentials
const char* ssid = "Airtel_BENIWAL?s";
const char* password = "HaR@1173";

// MQTT broker credentials
const char* mqtt_server = "lca10f4d.ala.dedicated.aws.emqxcloud.com";
const int mqtt_port = 1883;
const char* mqtt_user = "hiteshbeniwal";   
const char* mqtt_pass = "hiteshbeniwal";   
const char* mqtt_topic_wave = "SIT210/wave";
const char* mqtt_topic_pat = "SIT210/pat";

// Ultrasonic sensor pins
const int trigPin = 3;
const int echoPin = 2;

// LED pin
const int ledPin = 10;

// Debounce settings
bool messageSent = false;
unsigned long lastDebounceTime = 0;
const long debounceDelay = 1000; 

// Wi-Fi and MQTT client
WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup() {
  Serial.begin(9600);

  // Pin modes for sensor and LED
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // Connect to Wi-Fi and MQTT
  connectWiFi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  connectMQTT();
}

void loop() {
  if (!client.connected()) {
    connectMQTT();
  }
  
  client.loop();
  
  long distance = measureDistance();

  if (shouldSendMessage(distance)) {
    publishMessage("Hitesh");
    flashLED(3, 500);
    messageSent = true;
    lastDebounceTime = millis();
  } else if (distance >= 10 || distance == 0) {
    messageSent = false;
  }

  delay(1000);
}

// Function to connect to Wi-Fi
void connectWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi!");
  } else {
    Serial.println("Failed to connect to WiFi.");
  }
}

// Function to connect to MQTT
void connectMQTT() {
  Serial.print("Connecting to MQTT...");
  
  while (!client.connected()) {
    if (client.connect("ArduinoClient", mqtt_user, mqtt_pass)) {
      Serial.println("Connected to MQTT!");
      client.subscribe(mqtt_topic_pat);
    } else {
      Serial.print("Failed to connect, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

// Callback function for incoming MQTT messages
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received: ");
  
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  // If the topic is for 'pat', flash the LED 5 times
  if (String(topic) == mqtt_topic_pat) {
    flashLED(5, 200);
  }
}

// Measure distance using ultrasonic sensor
long measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = (duration / 2) / 29.1; 

  Serial.print("Distance: ");
  Serial.println(distance);
  
  return distance;
}

// Check if message should be sent based on distance and debounce time
bool shouldSendMessage(long distance) {
  unsigned long currentTime = millis();
  return (distance < 10 && distance > 0 && !messageSent && (currentTime - lastDebounceTime > debounceDelay));
}

// Publish MQTT message
void publishMessage(const char* message) {
  Serial.print("Publishing message: ");
  Serial.println(message);
  client.publish(mqtt_topic_wave, message);
}

// Flash the LED a given number of times
void flashLED(int times, int delayTime) {
  for (int i = 0; i < times; i++) {
    digitalWrite(ledPin, HIGH);
    delay(delayTime);
    digitalWrite(ledPin, LOW);
    delay(delayTime);
  }
}
