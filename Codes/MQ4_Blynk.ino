#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_TEMPLATE_NAME "MQ-4 Gas Monitor"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// WiFi credentials
char ssid[] = "YourWiFiSSID";
char pass[] = "YourWiFiPassword";

BlynkTimer timer;

void sendSensorData() {
  int analogValue = analogRead(A0);

  // Estimate Methane PPM (demo only – not calibrated)
  float ppm = analogValue * (1000.0 / 1023.0);

  // Determine gas quality level
  String gasQuality;

  if (ppm <= 200) {
    gasQuality = "Safe";
  } else if (ppm <= 400) {
    gasQuality = "Low Risk";
  } else if (ppm <= 600) {
    gasQuality = "Moderate Risk";
  } else if (ppm <= 800) {
    gasQuality = "High Risk";
  } else if (ppm <= 1000) {
    gasQuality = "Very High Risk";
  } else {
    gasQuality = "Dangerous";
  }

  // Print to Serial
  Serial.print("Raw: ");
  Serial.print(analogValue);
  Serial.print(" | PPM: ");
  Serial.print(ppm);
  Serial.print(" | Level: ");
  Serial.println(gasQuality);

  // Send to Blynk
  Blynk.virtualWrite(V0, analogValue);
  Blynk.virtualWrite(V1, ppm);
  Blynk.virtualWrite(V2, gasQuality);
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(2000L, sendSensorData);  // Run every 2 seconds
}

void loop() {
  Blynk.run();
  timer.run();
}
