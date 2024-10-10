//---------SUBSCRIBER CODE: This code listens for messages on an MQTT topic and performs actions based on the received messages (in your case, flashing an LED).-------

#include <WiFiNINA.h>
#include <PubSubClient.h>

// WiFi credentials and MQTT server settings
const char* ssid = "abcd";           // WiFi SSID
const char* password = "12345678";     // WiFi password
const char* mqtt_server = "broker.emqx.io";  // EMQX MQTT broker
const int mqtt_port = 1883;
const char* mqtt_topic_wave = "SIT210/wave";  // MQTT topic

// LED pin
const int ledPin = 8;

// Instances for WiFiClient and PubSubClient
WiFiClient espClient;
PubSubClient client(espClient);

// Function to connect to WiFi
void connectToWiFi(const char* ssid, const char* password) {
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to the WiFi network");
}

// Function to flash LED a specified number of times
void flashLED(int pin, int times, int delayDuration) {
    for (int i = 0; i < times; i++) {
        digitalWrite(pin, HIGH);
        delay(delayDuration);
        digitalWrite(pin, LOW);
        delay(delayDuration);
    }
}

// Function to handle incoming MQTT messages
void handleMQTTMessage(char* topic, byte* message, unsigned int length) {
    Serial.print("Message arrived on topic ");
    Serial.print(topic);
    Serial.print(": ");
    
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char)message[i]);
    }
    Serial.println();

    // Flash LED 3 times on message reception
    flashLED(ledPin, 3, 500);
}

// Function to connect to MQTT broker and subscribe to a topic
void connectToMQTT() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect with a client ID
        if (client.connect("ArduinoSubscriber")) {
            Serial.println("connected to MQTT broker");
            client.subscribe(mqtt_topic_wave);  // Subscribe to the topic
            Serial.print("Subscribed to topic: ");
            Serial.println(mqtt_topic_wave);
        } else {
            Serial.print("failed with state: ");
            Serial.print(client.state());
            delay(5000);  // Retry every 5 seconds if connection fails
        }
    }
}

void setup() {
    // Initialize serial communication for debugging
    Serial.begin(9600);

    // Initialize the LED pin as an output
    pinMode(ledPin, OUTPUT);

    // Connect to WiFi
    connectToWiFi(ssid, password);

    // Set MQTT server and callback function for incoming messages
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(handleMQTTMessage);
}

void loop() {
    // Ensure the client is connected to the MQTT broker
    if (!client.connected()) {
        connectToMQTT();
    }

    // Handle incoming MQTT messages
    client.loop();
}

