#define BLYNK_TEMPLATE_ID "TMPL3v9ncH78J"
#define BLYNK_TEMPLATE_NAME "Air quality monitoring"
#define BLYNK_AUTH_TOKEN "DWfRjtmT3Oqt1Vu4uh6DhE4yzl4ok8wL"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>

#include <DHT.h>

//#include <Wire.h> 


              
char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "AIR";  // type your wifi name
char pass[] = "aaaaaaaa";  // type your wifi password

BlynkTimer timer;

int gas = A0;
int sensorThreshold = 100;

#define DHTPIN 2 //Connect Out pin to D4 in NODE MCU
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
   int analogSensor = analogRead(gas);
  Blynk.virtualWrite(V2, analogSensor);
  Serial.print("Gas Value: ");
    Serial.println(analogSensor);
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
  
 //pinMode(gas, INPUT);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(3000L, sendSensor);

 //Wire.begin();




  }

void loop()
{
  Blynk.run();
  timer.run();
 float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
    int gasValue = analogRead(gas);

   if(gasValue > 600){

    Blynk.logEvent("pollution_alert","Bad Air");
  }
     if(t > 40){

    Blynk.logEvent("temp_alert","Temperature above 40 degree!!");
  }
 }