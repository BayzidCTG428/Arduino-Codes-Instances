#define PIR_PIN D5           // PIR output
#define TRIG_PIN D6          // Ultrasonic trigger
#define ECHO_PIN D7          // Ultrasonic echo
#define LED_PIN D4           // LED pin (GPIO2 - onboard LED on many boards)

#define DISTANCE_THRESHOLD 50  // cm, adjust as needed

void setup() {
  Serial.begin(9600);

  pinMode(PIR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW); // LED initially OFF

  Serial.println("PIR + Ultrasonic + LED Control");
  Serial.println("----------------------------------");
}

void loop() {
  // --- PIR Sensor ---
  int pirState = digitalRead(PIR_PIN);
  bool motionDetected = (pirState == HIGH);

  if (motionDetected) {
    Serial.println("PIR: Motion detected!");
  } else {
    Serial.println("PIR: No motion");
  }

  // --- Ultrasonic Sensor ---
  long duration;
  float distance;

  // Trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo time
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;  // in cm

  Serial.print("Ultrasonic Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // --- LED Control ---
  if (motionDetected && distance < DISTANCE_THRESHOLD) {
    digitalWrite(LED_PIN, HIGH);  // LED ON
    Serial.println("LED: ON (Both sensors active)");
  } else {
    digitalWrite(LED_PIN, LOW);   // LED OFF
    Serial.println("LED: OFF (One or both sensors inactive)");
  }

  Serial.println("----------------------------------");
  delay(2000); // Delay between readings
}
