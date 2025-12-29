//Tech Trends Shameer
#define BLYNK_TEMPLATE_ID "TMPL6hQXUHbYa"
#define BLYNK_TEMPLATE_NAME "Temperature and Humidity"
#define BLYNK_AUTH_TOKEN "cMIXZYG4-UFAiD7ExadTu08s4dNj0NcU"

#define BLYNK_PRINT Serial
//#include <WiFi.h>
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>

#include <DHT.h>


char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Event Horizon";  // type your wifi name
char pass[] = "Singularity_TON168";  // type your wifi password

BlynkTimer timer;


#define DHTPIN D6 //Connect Out pin to D2 in NODE MCU
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);


void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
    Blynk.virtualWrite(V0, t);
    Blynk.virtualWrite(V1, h);
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.print("    Humidity : ");
    Serial.println(h);
}
void setup()
{   
  
  Serial.begin(115200);
  

  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(100L, sendSensor);
 
  }

void loop()
{
  Blynk.run();
  timer.run();
 }