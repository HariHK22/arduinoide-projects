#include <DHT.h>
#define BLYNK_AUTH_TOKEN "YH9D4yOR5ubshZXXtoKC0xLTI-elVB2a"
// Your WiFi Credentials.
// Set password to "" for open networks.
char ssid[] = "Redmi 9 Power";
char pass[] = "lee123sak456";


// define the GPIO connected with Sensors & LEDs
#define MQ2_SENSOR    A0 //A0
#define RAIN_SENSOR   5  //D1
#define GREEN_LED     14 //D5
#define RED_LED       13 //D7
#define WIFI_LED      16 //D0

//#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
int relaypin = D4;
int MQ2_SENSOR_Value = 0;
int RAIN_SENSOR_Value = 0;
bool isconnected = false;
char auth[] = BLYNK_AUTH_TOKEN;

#define VPIN_BUTTON_1    V1 
#define VPIN_BUTTON_2    V2

BlynkTimer timer;

void checkBlynkStatus() { // called every 2 seconds by SimpleTimer
  getSensorData();
  isconnected = Blynk.connected();
  if (isconnected == true) {
    digitalWrite(WIFI_LED, LOW);
    sendSensorData();
    //Serial.println("Blynk Connected");
  }
  else{
    digitalWrite(WIFI_LED, HIGH);
    Serial.println("Blynk Not Connected");
  }
}

void getSensorData()
{
  MQ2_SENSOR_Value = map(analogRead(MQ2_SENSOR), 0, 1024, 0, 100);
  RAIN_SENSOR_Value = digitalRead(RAIN_SENSOR);
  if (MQ2_SENSOR_Value > 50 ){
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  }
  else if (RAIN_SENSOR_Value == 0 ){
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  }
  else{
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  }
}

void sendSensorData()
{  
  Blynk.virtualWrite(VPIN_BUTTON_1, MQ2_SENSOR_Value);
  if (MQ2_SENSOR_Value > 50 )
  {
    Blynk.logEvent("gas", "Gas Detected!");
  }
  else if (RAIN_SENSOR_Value == 0 )
  {
    Blynk.logEvent("poop_or_pee", "POOP or PEE detected!");
    Blynk.virtualWrite(VPIN_BUTTON_2, "POOP or PEE detected!");
  }
  else if (RAIN_SENSOR_Value == 1 )
  {
    Blynk.virtualWrite(VPIN_BUTTON_2, "No POOP or PEE detected.");
  } 
} 
  #define DHTPIN 4 //Connect Out pin to D2 in NODE MCU
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
    Blynk.virtualWrite(V3, h);
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.print("    Humidity : ");
    Serial.println(h);
     if(t > 35){
    Blynk.logEvent("temp_alert","Temp above 35 degree");
  }
}




void setup()
{
  Serial.begin(9600);
  dht.begin();
  timer.setInterval(100L, sendSensor);
  pinMode(MQ2_SENSOR, INPUT);
  pinMode(RAIN_SENSOR, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(WIFI_LED, OUTPUT);
  pinMode(relaypin,OUTPUT);


  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(WIFI_LED, HIGH);

  WiFi.begin(ssid, pass);
  timer.setInterval(2000L, checkBlynkStatus); // check if Blynk server is connected every 2 seconds
  Blynk.config(auth);
  delay(1000);
}

void loop()
{
  Blynk.run();
  timer.run();
}
