#include <ESP8266WiFi.h>                 
#include <FirebaseArduino.h>      
#include <DHT.h> 


// Wi-Fi credentials
#define WIFI_SSID "Event Horizon"
#define WIFI_PASSWORD "Singularity_TON168"

// Firebase project credentials
#define FIREBASE_HOST "cold-storage-2025-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyBG7JPR9QJtni9L1aEOUred1ZO5vbObyq0"  // Or your Web API key

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// DHT11 setup
#define DHTPIN D6
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Timer
unsigned long lastSendTime = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected!");

  // Firebase setup
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  dht.begin();
}

void loop() {
  if (millis() - lastSendTime >= 5000) {
    lastSendTime = millis();

    // --- DHT11 ---
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (!isnan(t) && !isnan(h)) {
      Serial.printf("Temp: %.2f°C | Hum: %.2f%%\n", t, h);
      Firebase.setFloat(fbdo, "/DHT11/Temperature", t);
      Firebase.setFloat(fbdo, "/DHT11/Humidity", h);
    }

    // --- MQ-135 ---
    int mq135_raw = analogRead(A0);
    float mq135_ppm = mq135_raw * (1000.0 / 1023.0);
    String airQuality = classifyAirQuality(mq135_ppm);

    Serial.printf("MQ-135 Raw: %d | CO2: %.2f ppm | %s\n", mq135_raw, mq135_ppm, airQuality.c_str());

    Firebase.setInt(fbdo, "/MQ135/Raw", mq135_raw);
    Firebase.setFloat(fbdo, "/MQ135/PPM", mq135_ppm);
    Firebase.setString(fbdo, "/MQ135/Quality", airQuality);

    delay(500);  // Brief pause if reusing A0

    // --- MQ-4 ---
    int mq4_raw = analogRead(A0);
    float mq4_ppm = mq4_raw * (1000.0 / 1023.0);
    String gasRisk = classifyMethaneRisk(mq4_ppm);

    Serial.printf("MQ-4   Raw: %d | CH4: %.2f ppm | %s\n", mq4_raw, mq4_ppm, gasRisk.c_str());

    Firebase.setInt(fbdo, "/MQ4/Raw", mq4_raw);
    Firebase.setFloat(fbdo, "/MQ4/PPM", mq4_ppm);
    Firebase.setString(fbdo, "/MQ4/Risk", gasRisk);
  }
}

// --- Classification functions ---
String classifyAirQuality(float ppm) {
  if (ppm <= 200) return "Good";
  else if (ppm <= 400) return "Moderate";
  else if (ppm <= 600) return "Unhealthy for Sensitive Groups";
  else if (ppm <= 800) return "Unhealthy";
  else if (ppm <= 1000) return "Very Unhealthy";
  else return "Hazardous";
}

String classifyMethaneRisk(float ppm) {
  if (ppm <= 200) return "Safe";
  else if (ppm <= 400) return "Low Risk";
  else if (ppm <= 600) return "Moderate Risk";
  else if (ppm <= 800) return "High Risk";
  else if (ppm <= 1000) return "Very High Risk";
  else return "Dangerous";
}
