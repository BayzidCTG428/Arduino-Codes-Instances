#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_TEMPLATE_NAME "MQ-135 Air Monitor"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// WiFi credentials
char ssid[] = "YourWiFiSSID";
char pass[] = "YourWiFiPassword";

BlynkTimer timer;

void sendSensorData() {
  int analogValue = analogRead(A0);

  // Estimate CO2 PPM (very rough – for demo purposes only)
  float ppm = analogValue * (1000.0 / 1023.0);

  // Determine air quality level
  String airQuality;

  if (ppm <= 200) {
    airQuality = "Good";
  } else if (ppm <= 400) {
    airQuality = "Moderate";
  } else if (ppm <= 600) {
    airQuality = "Unhealthy for Sensitive Groups";
  } else if (ppm <= 800) {
    airQuality = "Unhealthy";
  } else if (ppm <= 1000) {
    airQuality = "Very Unhealthy";
  } else {
    airQuality = "Hazardous";
  }

  // Debug output to Serial Monitor
  Serial.print("Raw Value: ");
  Serial.print(analogValue);
  Serial.print(" | CO2 PPM: ");
  Serial.print(ppm);
  Serial.print(" | Air Quality: ");
  Serial.println(airQuality);

  // Send values to Blynk app
  Blynk.virtualWrite(V0, analogValue);
  Blynk.virtualWrite(V1, ppm);
  Blynk.virtualWrite(V2, airQuality);
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(2000L, sendSensorData);  // Send every 2 seconds
}

void loop() {
  Blynk.run();
  timer.run();
}

