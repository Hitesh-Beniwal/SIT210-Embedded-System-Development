#include <WiFiNINA.h>
#include <Wire.h>
#include <BH1750.h>

// Constants for WiFi and IFTTT
const char ssid[] = "Airtel_BENIWAL?s";  // Your WiFi SSID
const char pass[] = "HaR@1173";          // Your WiFi password
const char host[] = "maker.ifttt.com";  // IFTTT host
const String apiKey = "xG7bp9Ndi-tDF4LH8GGKQNpZFMQ2sIPXesHLSdkOSN"; // IFTTT API key

// Constants for light levels
const float sunlightThreshold = 1000; // Lux level for direct sunlight
const float sundownThreshold = 300;   // Lux level for low light/sundown

// Hardware components
BH1750 lightMeter; // Light sensor
WiFiClient client; // WiFi client
const int ledPin = 2; // LED indicator pin

// Setup function
void setup() {
  Serial.begin(9600); // Start serial communication
  pinMode(ledPin, OUTPUT); // Initialize LED pin
  Wire.begin(); // Initialize I2C
  lightMeter.begin(); // Initialize light sensor
  connectWiFi(); // Connect to WiFi
}

// Main loop
void loop() {
  float lux = readLightLevel(); // Read the light level
  handleLightLevel(lux);        // Evaluate light level and trigger webhook
  delay(60000); // Wait for 1 minute before reading again
}

// Connect to WiFi
void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000); // Wait for 1 second before retrying
  }

  Serial.println("\nConnected to WiFi");
}

// Read the light level from the sensor
float readLightLevel() {
  float lux = lightMeter.readLightLevel();
  Serial.print("Lux: ");
  Serial.println(lux);
  return lux;
}

// Handle light level and trigger the appropriate webhook
void handleLightLevel(float lux) {
  if (lux >= sunlightThreshold) {
    triggerWebhook("Sunlight");
  } else if (lux < sundownThreshold) {
    triggerWebhook("Sundown");
  }
}

// Trigger the IFTTT webhook
void triggerWebhook(const String& eventName) {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi(); // Reconnect if WiFi is disconnected
  }

  String url = "/trigger/" + eventName + "/with/key/" + apiKey;
  sendGETRequest(url);
}

// Send the HTTP GET request to IFTTT
void sendGETRequest(const String& url) {
  if (client.connect(host, 80)) {
    client.println("GET " + url + " HTTP/1.1");
    client.println("Host: " + String(host));
    client.println("Connection: close");
    client.println();
    Serial.println("Webhook triggered successfully");
    delay(500); // Wait for server response
  } else {
    Serial.println("Failed to connect to webhook server");
  }
  client.stop(); // Close the connection
}
