// Combined NodeMCU Code: DHT11, MQ135, LDR with Buzzer + PIR, Ultrasonic with External LED

#include <DHT.h>

// ---- Pin Definitions ----
#define DHTPIN D4           // DHT11 sensor
#define DHTTYPE DHT11
#define LDR_PIN D0          // LDR input
#define BUZZER_PIN D2       // Buzzer output
#define MQ135_PIN A0        // MQ-135 analog input

#define PIR_PIN D5          // PIR motion sensor
#define TRIG_PIN D6         // Ultrasonic trigger
#define ECHO_PIN D7         // Ultrasonic echo
#define LED_PIN D1          // External LED

// ---- Thresholds ----
#define LDR_THRESHOLD 400     // Light threshold for darkness
#define DISTANCE_THRESHOLD 50 // cm for ultrasonic distance

// ---- Initialize DHT Sensor ----
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);

  dht.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  Serial.println("NodeMCU Multi-Sensor Monitoring System");
  Serial.println("Sensors: DHT11, LDR, MQ-135, PIR, Ultrasonic");
  Serial.println("--------------------------------------------");
}

void loop() {
  // --- Read DHT11 Temperature & Humidity ---
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT11!");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C | Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  }

  // --- Read LDR Light Level ---
  int lightLevel = analogRead(LDR_PIN);
  Serial.print("Light Level: ");
  Serial.println(lightLevel);

  // --- Buzzer Control ---
  if (lightLevel < LDR_THRESHOLD) {
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println("It's Bright. Buzzer OFF");
  } else {
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("It's DARK! Buzzer ON");
  }

  // --- MQ-135 Gas Sensor ---
  int gasValue = analogRead(MQ135_PIN);
  float ppm = gasValue * (1000.0 / 1023.0);
  String airQuality;
  if (ppm <= 200) airQuality = "Good";
  else if (ppm <= 400) airQuality = "Moderate";
  else if (ppm <= 600) airQuality = "Unhealthy for Sensitive Groups";
  else if (ppm <= 800) airQuality = "Unhealthy";
  else if (ppm <= 1000) airQuality = "Very Unhealthy";
  else airQuality = "Hazardous";

  Serial.print("MQ-135: Raw=");
  Serial.print(gasValue);
  Serial.print(" | CO2 PPM: ");
  Serial.print(ppm);
  Serial.print(" | Air Quality: ");
  Serial.println(airQuality);

  // --- PIR Motion Detection ---
  int pirState = digitalRead(PIR_PIN);
  bool motionDetected = (pirState == HIGH);
  if (motionDetected) {
    Serial.println("PIR: Motion detected!");
  } else {
    Serial.println("PIR: No motion");
  }

  // --- Ultrasonic Distance Measurement ---
  long duration;
  float distance;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Ultrasonic Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // --- LED Control Based on PIR and Ultrasonic ---
  if (motionDetected && distance < DISTANCE_THRESHOLD) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED: ON (Both sensors active)");
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED: OFF (One or both sensors inactive)");
  }

  Serial.println("--------------------------------------------");
  delay(2000);
}
