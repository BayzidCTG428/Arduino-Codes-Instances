// Tech Trends Shameer - Merged Blynk IoT Project

#define BLYNK_TEMPLATE_ID "TMPL6pbdU3i37"
#define BLYNK_TEMPLATE_NAME "Digital Cold Storage Management System"
#define BLYNK_AUTH_TOKEN "xhGaywVqMu4MPlk9CCaruFISxNunXKSS"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// WiFi credentials
char ssid[] = "Event Horizon";
char pass[] = "Singularity_TON168";

// DHT setup
#define DHTPIN D6
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Blynk timer
BlynkTimer timer;

void readDHTSensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" °C, Humidity: ");
  Serial.print(h);
  Serial.println(" %");

  Blynk.virtualWrite(V0, t); // Temperature
  Blynk.virtualWrite(V1, h); // Humidity
}

void readMQ135() {
  int analogValue = analogRead(A0);
  float ppm = analogValue * (1000.0 / 1023.0);

  String airQuality;
  if (ppm <= 200) airQuality = "Good";
  else if (ppm <= 400) airQuality = "Moderate";
  else if (ppm <= 600) airQuality = "Unhealthy for Sensitive Groups";
  else if (ppm <= 800) airQuality = "Unhealthy";
  else if (ppm <= 1000) airQuality = "Very Unhealthy";
  else airQuality = "Hazardous";

  Serial.print("MQ-135 | Raw: ");
  Serial.print(analogValue);
  Serial.print(" | CO2 PPM: ");
  Serial.print(ppm);
  Serial.print(" | Quality: ");
  Serial.println(airQuality);

  Blynk.virtualWrite(V2, analogValue);  // Raw Value
  Blynk.virtualWrite(V3, ppm);          // Estimated PPM
  Blynk.virtualWrite(V4, airQuality);   // Air Quality Status
}

void readMQ4() {
  // If using both MQ-135 and MQ-4 on A0, you'll need a multiplexer or switch to read separately.
  // For demo, we simulate MQ-4 data just after MQ-135.
  delay(500);  // brief pause between reads

  int analogValue = analogRead(A0);
  float ppm = analogValue * (1000.0 / 1023.0);

  String gasQuality;
  if (ppm <= 200) gasQuality = "Safe";
  else if (ppm <= 400) gasQuality = "Low Risk";
  else if (ppm <= 600) gasQuality = "Moderate Risk";
  else if (ppm <= 800) gasQuality = "High Risk";
  else if (ppm <= 1000) gasQuality = "Very High Risk";
  else gasQuality = "Dangerous";

  Serial.print("MQ-4   | Raw: ");
  Serial.print(analogValue);
  Serial.print(" | CH4 PPM: ");
  Serial.print(ppm);
  Serial.print(" | Risk: ");
  Serial.println(gasQuality);

  Blynk.virtualWrite(V5, analogValue);  // Raw Value
  Blynk.virtualWrite(V6, ppm);          // Estimated PPM
  Blynk.virtualWrite(V7, gasQuality);   // Gas Quality Status
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();

  timer.setInterval(2000L, readDHTSensor);
  timer.setInterval(5000L, readMQ135);
  timer.setInterval(8000L, readMQ4); // Staggered to avoid analogRead conflict
}

void loop() {
  Blynk.run();
  timer.run();
}
