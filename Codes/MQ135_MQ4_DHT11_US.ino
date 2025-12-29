#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <DHT.h>
#include <Servo.h>

#define FIREBASE_HOST "cold-storage-2025-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyBG7JPR9QJtni9L1aEOUred1ZO5vbObyq0"
#define WIFI_SSID "Event Horizon"
#define WIFI_PASSWORD "Singularity_TON168"

// ----- DHT11 -----
#define DHTPIN D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ----- Ultrasonic Sensor -----
#define TRIG_PIN D5
#define ECHO_PIN D6

// ----- Actuators -----
#define BUZZER_PIN D3
#define LED_PIN D4
#define SERVO_PIN D7
Servo doorServo;

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  doorServo.attach(SERVO_PIN);
  doorServo.write(0); // Door closed

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected! IP Address: " + WiFi.localIP().toString());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  // ----- DHT11 -----
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (!isnan(h) && !isnan(t)) {
    Serial.printf("Humidity: %.2f%%  Temperature: %.2f°C\n", h, t);
    Firebase.pushString("/DHT11/Humidity", String(h));
    Firebase.pushString("/DHT11/Temperature", String(t));
    if (Firebase.failed()) {
      Serial.println("DHT11 push failed: " + Firebase.error());
    }
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }

  // ----- MQ-4 -----
  int mq4Value = analogRead(A0);
  float mq4ppm = mq4Value * (1000.0 / 1023.0);
  String gasQuality = (mq4ppm <= 200) ? "Safe" :
                      (mq4ppm <= 400) ? "Low Risk" :
                      (mq4ppm <= 600) ? "Moderate Risk" :
                      (mq4ppm <= 800) ? "High Risk" :
                      (mq4ppm <= 1000) ? "Very High Risk" : "Dangerous";

  Serial.printf("MQ-4 CH4 - PPM: %.2f | Quality: %s\n", mq4ppm, gasQuality.c_str());
  Firebase.pushFloat("/MQ4/PPM", mq4ppm);
  Firebase.pushString("/MQ4/Quality", gasQuality);
  if (Firebase.failed()) {
    Serial.println("MQ-4 push failed: " + Firebase.error());
  }

  delay(3000); // Delay before next sensor

  // ----- MQ-135 -----
  int mq135Value = analogRead(A0);
  float mq135ppm = mq135Value * (1000.0 / 1023.0);
  String airQuality = (mq135ppm <= 200) ? "Good" :
                      (mq135ppm <= 400) ? "Moderate" :
                      (mq135ppm <= 600) ? "Unhealthy for Sensitive Groups" :
                      (mq135ppm <= 800) ? "Unhealthy" :
                      (mq135ppm <= 1000) ? "Very Unhealthy" : "Hazardous";

  Serial.printf("MQ-135 CO2 - PPM: %.2f | Air Quality: %s\n", mq135ppm, airQuality.c_str());
  Firebase.pushFloat("/MQ135/PPM", mq135ppm);
  Firebase.pushString("/MQ135/Quality", airQuality);
  if (Firebase.failed()) {
    Serial.println("MQ-135 push failed: " + Firebase.error());
  }

  delay(3000);

  // ----- Ultrasonic Sensor -----
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;

  Serial.printf("Ultrasonic Distance: %.2f cm\n", distance);

  if (distance > 0 && distance < 15) {
    doorServo.write(90); // open door
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
    Firebase.setString("/DoorStatus", "Door Opened");
    Serial.println("Door Opened");
  } else {
    doorServo.write(0); // close door
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    Firebase.setString("/DoorStatus", "Door Closed");
    Serial.println("Door Closed");
  }

  if (Firebase.failed()) {
    Serial.println("Ultrasonic/Servo push failed: " + Firebase.error());
  }

  delay(5000);
}
