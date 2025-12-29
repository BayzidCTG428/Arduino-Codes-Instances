// Pins
const int ldrPin = A0;           // LDR analog pin
const int ldrLedPin = 8;         // LED controlled by LDR

const int trigPin = 9;           // Ultrasonic trigger pin
const int echoPin = 10;          // Ultrasonic echo pin
const int ultrasonicLedPin = 7;  // LED controlled by ultrasonic sensor

// Thresholds
const int ldrThreshold = 500;    // Adjust based on light conditions (0-1023)
const long distanceThreshold = 20; // Distance in cm to trigger ultrasonic LED

bool ldrLedState = false; // Track LDR LED state

void setup() {
  Serial.begin(9600);

  pinMode(ldrLedPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ultrasonicLedPin, OUTPUT);
}

void loop() {
  // ----- Section 1: LDR sensor controlling LED -----
  int ldrValue = analogRead(ldrPin);
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  if (ldrValue < ldrThreshold) {
    ldrLedState = true; // Once dark, set state to true
  }
  /*else{
    ldrLedState = false;
  }*/

  // Keep LED ON once it has turned on
  digitalWrite(ldrLedPin, ldrLedState ? HIGH : LOW);

  // ----- Section 2: Ultrasonic sensor controlling another LED -----
  long duration, distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 0 && distance < distanceThreshold) {
    digitalWrite(ultrasonicLedPin, HIGH); // Object is close, turn LED ON
  } else {
    digitalWrite(ultrasonicLedPin, LOW);  // No close object, turn LED OFF
  }

  delay(2000);
}