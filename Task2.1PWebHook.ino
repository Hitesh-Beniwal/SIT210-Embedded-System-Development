#include <WiFiNINA.h>
#include <DHT.h>
#include <ThingSpeak.h>

// DHT Sensor settings
#define DHTPIN 2          // Digital pin connected to the DHT22
#define DHTTYPE DHT22     // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);

// WiFi settings
char ssid[] = "wifi_name";             // your network SSID (name)
char pass[] = "wifi_password";           // your network password

WiFiClient client;

// ThingSpeak settings
unsigned long myChannelNumber = channel_id; // Replace with your ThingSpeak channel ID
const char * myWriteAPIKey = "API_Key";  // Replace with your ThingSpeak Write API Key

void setup() {
  Serial.begin(9600);
  while (!Serial);

  dht.begin();

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  // Read temperature as Celsius
  float temp = dht.readTemperature();
  // Read humidity
  float humidity = dht.readHumidity();

  // Check if any reads failed and exit early (to try again).
  if (isnan(temp) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print values to serial monitor
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" *C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Update ThingSpeak channel with temperature and humidity
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, humidity);

  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel Updated!!");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  delay(60000);  // Wait 60 seconds to update again
}
