#include <DHT.h>

// Define DHT pin and type
#define DHTPIN D4        // D4 on NodeMCU is GPIO2
#define DHTTYPE DHT11    // DHT 11

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.println("DHT11 sensor reading...");
}

void loop() {
  // Wait a few seconds between measurements
  delay(2000);

  // Read temperature as Celsius
  float temp = dht.readTemperature();
  // Read humidity
  float hum = dht.readHumidity();

  // Check if readings are valid
  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print values to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" °C | Humidity: ");
  Serial.print(hum);
  Serial.println(" %");
}
