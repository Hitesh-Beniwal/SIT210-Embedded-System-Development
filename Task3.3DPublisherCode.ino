//---------------PUBLISHER CODE: This code sends messages to a specific MQTT topic based on certain conditions (like detecting a wave with an ultrasonic sensor).------

#include <WiFiNINA.h>
#include <PubSubClient.h>

// WiFi credentials and MQTT server settings
const char* ssid = "Vansh";           // WiFi SSID
const char* password = "12345678";     // WiFi password
const char* mqtt_server = "broker.emqx.io";  // EMQX MQTT broker
const int mqtt_port = 1883;
const char* mqtt_topic_wave = "SIT210/wave";  // MQTT topic

// Ultrasonic Sensor pins
const int trigPin = 9;
const int echoPin = 10;
long duration;
int distance;

// Instances for WiFiClient and PubSubClient
WiFiClient espClient;
PubSubClient client(espClient);

// Function to initialize ultrasonic sensor
void setupUltrasonicSensor(int trigPin, int echoPin) {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

// Function to handle WiFi connection
void connectToWiFi(const char* ssid, const char* password) {
    Serial.print("Connecting to WiFi");
    
    // Attempt to connect until successful
    while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConnected to WiFi");
}

// Function to handle MQTT connection
void connectToMQTT() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect to MQTT broker
        if (client.connect("ArduinoPublisher")) {
            Serial.println("Connected to MQTT broker");
        } else {
            Serial.print("MQTT connection failed, retrying in 5 seconds...");
            delay(5000);  // Wait 5 seconds before retrying
        }
    }
}

// Function to read ultrasonic sensor distance in cm
int readUltrasonicDistance(int trigPin, int echoPin) {
    // Send trigger pulse
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);       // Ensure trigger is off for at least 2 µs
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);      // Trigger pulse duration (10 µs)
    digitalWrite(trigPin, LOW);

    // Read echo pulse duration
    long duration = pulseIn(echoPin, HIGH);

    // Calculate distance in cm and return it
    return duration * 0.034 / 2;
}

// Function to publish a message to the MQTT topic
void publishMQTTMessage(const char* topic, const char* message) {
    if (client.publish(topic, message)) {
        Serial.println("Message published successfully");
    } else {
        Serial.println("Failed to publish message");
    }
}

// Function to detect waves and send MQTT message if detected
void detectWaveAndPublish() {
    int distance = readUltrasonicDistance(trigPin, echoPin);
    
    // Debug print to check actual readings
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // If distance is within 20 cm, consider it a wave
    if (distance > 0 && distance < 20) {
        publishMQTTMessage(mqtt_topic_wave, "Wave detected");
        delay(3000);  // Avoid publishing too frequently
    } else {
        Serial.println("No wave detected.");
    }
}

void setup() {
    // Initialize serial communication for debugging
    Serial.begin(9600);
    
    // Initialize ultrasonic sensor
    setupUltrasonicSensor(trigPin, echoPin);
    
    // Connect to WiFi
    connectToWiFi(ssid, password);
    
    // Setup MQTT server details
    client.setServer(mqtt_server, mqtt_port);
}

void loop() {
    // Ensure MQTT connection is alive
    if (!client.connected()) {
        connectToMQTT();
    }

    // Call loop to handle MQTT communication
    client.loop();

    // Check for wave detection and publish message if necessary
    detectWaveAndPublish();
}
