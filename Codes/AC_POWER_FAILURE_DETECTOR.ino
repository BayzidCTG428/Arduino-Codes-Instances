const int sensorPin = A0;
const int ledPin = 13;
const int buzzerPin = 12;
int sensorValue = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  
  if (sensorValue > 100) { // AC present
    digitalWrite(ledPin, HIGH);
    noTone(buzzerPin); // Turn off buzzer
  } else { // AC failure
    digitalWrite(ledPin, LOW);

    // Siren effect: sweep frequency up and down
    for (int freq = 1000; freq <= 2000; freq += 50) {
      tone(buzzerPin, freq);
      delay(10);
    }
    for (int freq = 2000; freq >= 1000; freq -= 50) {
      tone(buzzerPin, freq);
      delay(10);
    }
  }

  delay(500);
}
