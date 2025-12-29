#define LDR_PIN D0       // Analog pin connected to LDR voltage divider
#define BUZZER_PIN D1    // Digital pin connected to buzzer (GPIO5)
#define THRESHOLD 400    // Light level threshold (adjust this)

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("LDR Sensor with Buzzer - NodeMCU");
}

void loop() {
  int lightLevel = analogRead(LDR_PIN); // Read LDR value (0–1023)

  Serial.print("Light Level: ");
  Serial.println(lightLevel);

  // If it’s dark (light level below threshold), activate buzzer
  if (lightLevel < THRESHOLD) {
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println("It's Bright. Buzzer OFF");
  } else {
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("It's DARK! Buzzer ON");
  }

  delay(1000); // Wait 1 second before next reading
}
