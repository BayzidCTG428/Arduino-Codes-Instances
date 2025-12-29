// MQ-135 Sensor with NodeMCU
// Shows raw value, estimated CO2 ppm, and air quality level

void setup() {
  Serial.begin(9600);
  Serial.println("MQ-135 Air Quality Monitor");
}

void loop() {
  int analogValue = analogRead(A0);  // Read from analog pin A0

  // Estimate PPM (very rough scaling – for demo purposes only)
  float ppm = analogValue * (1000.0 / 1023.0); // map 0–1023 to 0–1000 ppm

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

  // Print to Serial Monitor
  Serial.print("Raw Value: ");
  Serial.print(analogValue);
  Serial.print(" | Estimated CO2 PPM: ");
  Serial.print(ppm);
  Serial.print(" | Air Quality: ");
  Serial.println(airQuality);

  delay(2000); // wait before next read
}
