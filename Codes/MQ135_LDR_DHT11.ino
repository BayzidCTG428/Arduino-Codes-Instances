#include <DHT.h>

// ---- Pin Definitions ----
#define DHTPIN D4 
#define LDR_PIN D0          // DHT11 on D4 (GPIO2)
#define DHTTYPE DHT11
#define BUZZER_PIN D1      // Buzzer on D1 (GPIO5)
#define MQ135_PIN A0   // Shared analog pin for LDR and MQ-135


// ---- Global Threshold ----
#define LDR_THRESHOLD 400  // Adjust this for darkness detection

// ---- Initialize DHT ----
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(BUZZER_PIN, OUTPUT);

  Serial.println("NodeMCU Monitoring System");
  Serial.println("Sensors: DHT11, LDR, MQ-135 with Buzzer");
  Serial.println("---------------------------------------");
}

void loop() {
  // --- Read Temperature & Humidity ---
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

  // --- Read Analog Value (LDR / MQ-135 shared pin) ---
  int analogValue = analogRead(MQ135_PIN);
  int lightLevel = analogRead(LDR_PIN); // Read LDR value (0–1023)
  Serial.print("Light Level: ");
  Serial.println(lightLevel);
  // --- LDR + Buzzer Control ---
  if (lightLevel < LDR_THRESHOLD) {
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println("It's Bright. Buzzer OFF");
  } else {
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("It's DARK! Buzzer ON");
  }

  // --- MQ-135 Gas Estimation ---
  float ppm = analogValue * (1000.0 / 1023.0); // crude PPM scale
  String airQuality;

  if (ppm <= 200) airQuality = "Good";
  else if (ppm <= 400) airQuality = "Moderate";
  else if (ppm <= 600) airQuality = "Unhealthy for Sensitive Groups";
  else if (ppm <= 800) airQuality = "Unhealthy";
  else if (ppm <= 1000) airQuality = "Very Unhealthy";
  else airQuality = "Hazardous";

  Serial.print("MQ-135: Raw=");
  Serial.print(analogValue);
  Serial.print(" | CO2 PPM: ");
  Serial.print(ppm);
  Serial.print(" | Air Quality: ");
  Serial.println(airQuality);

  Serial.println("---------------------------------------");

  delay(2000);  // Wait 2 seconds before next cycle
}
