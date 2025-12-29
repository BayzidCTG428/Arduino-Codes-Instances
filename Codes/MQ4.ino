// MQ-4 Sensor with NodeMCU
// Shows raw value, estimated CH4 ppm, and air quality level

void setup() {
  Serial.begin(9600);
  Serial.println("MQ-4 Methane Gas Monitor");
}

void loop() {
  int analogValue = analogRead(A0);  // Read from analog pin A0

  // Estimate Methane PPM (very rough scaling – for demo purposes only)
  float ppm = analogValue * (1000.0 / 1023.0); // map 0–1023 to 0–1000 ppm

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

  // Print to Serial Monitor
  Serial.print("Raw Value: ");
  Serial.print(analogValue);
  Serial.print(" | Estimated CH4 PPM: ");
  Serial.print(ppm);
  Serial.print(" | Gas Quality: ");
  Serial.println(gasQuality);

  delay(2000); // wait before next read
}
