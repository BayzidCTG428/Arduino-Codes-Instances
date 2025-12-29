#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <DHT.h>

#define FIREBASE_HOST "cold-storage-2025-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyBG7JPR9QJtni9L1aEOUred1ZO5vbObyq0"
#define WIFI_SSID "Event Horizon"
#define WIFI_PASSWORD "Singularity_TON168"

#define DHTPIN D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected! IP Address: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  // ----- DHT11 -----
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  } else {
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print("%  Temperature: ");
    Serial.print(t);
    Serial.println("°C");

    Firebase.pushString("/DHT11/Humidity", String(h));
    Firebase.pushString("/DHT11/Temperature", String(t));
    if (Firebase.failed()) {
      Serial.print("DHT11 push failed: ");
      Serial.println(Firebase.error());
    }
  }

  // ----- MQ-4 -----
  int mq4Value = analogRead(A0);
  float mq4ppm = mq4Value * (1000.0 / 1023.0);
  String gasQuality;
  if (mq4ppm <= 200) gasQuality = "Safe";
  else if (mq4ppm <= 400) gasQuality = "Low Risk";
  else if (mq4ppm <= 600) gasQuality = "Moderate Risk";
  else if (mq4ppm <= 800) gasQuality = "High Risk";
  else if (mq4ppm <= 1000) gasQuality = "Very High Risk";
  else gasQuality = "Dangerous";

  Serial.print("MQ-4 CH4 - PPM: ");
  Serial.print(mq4ppm);
  Serial.print(" | Quality: ");
  Serial.println(gasQuality);

  Firebase.pushFloat("/MQ4/PPM", mq4ppm);
  Firebase.pushString("/MQ4/Quality", gasQuality);
  if (Firebase.failed()) {
    Serial.print("MQ-4 push failed: ");
    Serial.println(Firebase.error());
  }

  delay(3000); // allow some delay before switching sensors

  // ----- MQ-135 -----
  int mq135Value = analogRead(A0); // reuse A0; assumes sensor switch or time sharing
  float mq135ppm = mq135Value * (1000.0 / 1023.0);
  String airQuality;
  if (mq135ppm <= 200) airQuality = "Good";
  else if (mq135ppm <= 400) airQuality = "Moderate";
  else if (mq135ppm <= 600) airQuality = "Unhealthy for Sensitive Groups";
  else if (mq135ppm <= 800) airQuality = "Unhealthy";
  else if (mq135ppm <= 1000) airQuality = "Very Unhealthy";
  else airQuality = "Hazardous";

  Serial.print("MQ-135 CO2 - PPM: ");
  Serial.print(mq135ppm);
  Serial.print(" | Air Quality: ");
  Serial.println(airQuality);

  Firebase.pushFloat("/MQ135/PPM", mq135ppm);
  Firebase.pushString("/MQ135/Quality", airQuality);
  if (Firebase.failed()) {
    Serial.print("MQ-135 push failed: ");
    Serial.println(Firebase.error());
  }

  delay(5000); // Delay between loops
}
