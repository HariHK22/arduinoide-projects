// Include the library files
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include<Servo.h>
#include <DHT.h>


Servo servo1, servo2;

// Define the motor pins
#define ENA D10
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4
#define ENB D5
#define relay1 D0



// Variables for the Blynk widget values
int x = 50;
int y = 50;
int Speed;
int smokeA0 = A0;
int data = 0;
int sensorThres = 100;



char auth[] = "keLDXVzMBO2khs5FfhL4-hgnEIzj15gp"; //Enter your Blynk auth token
char ssid[] = "HK"; //Enter your WIFI name
char pass[] = "aaaaaaaa"; //Enter your WIFI passowrd



#define DHTPIN D9// Mention the digital pin where you connected 
#define DHTTYPE DHT11     // DHT 11  
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void dhSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.println(t);
  Blynk.virtualWrite(V9, h);
  Blynk.virtualWrite(V8, t);
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.print("    Humidity : ");
    Serial.println(h);


  if(t > 30){
    Blynk.logEvent("temp_alert","Temp above 30 degree");
  }
}

void sendSensor(){
 
 int data = analogRead(smokeA0);
 Blynk.virtualWrite(V7, data);
  Serial.print("Pin A0: ");
  Serial.println(data);


  if(data > 20){

    Blynk.logEvent("gas_alert","Gas Leakage Detected");
  }
}

BLYNK_WRITE(V3)
{
  int s0 = param.asInt(); 
  servo1.write(s0);
  Blynk.virtualWrite(V5, s0);
}
BLYNK_WRITE(V4)
{
  int s1 = param.asInt(); 
  servo2.write(s1);
  Blynk.virtualWrite(V6, s1);
}
BLYNK_WRITE(V10) {
  bool value1 = param.asInt();
  // Check these values and turn the relay1 ON and OFF
  if (value1 == 1) {
    digitalWrite(relay1, LOW);
  } else {
    digitalWrite(relay1, HIGH);
  }
  
}


void setup() {
 
  //Set the motor pins as output pins
  pinMode(relay1, OUTPUT);
  pinMode(smokeA0, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  digitalWrite(relay1, HIGH);

  Serial.begin(115200);
  servo1.attach(D6);
  servo2.attach(D7);
    dht.begin();
  timer.setInterval(2500L, sendSensor);
    timer.setInterval(2500L, dhSensor);
// Initialize the Blynk library
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  delay(2000);

  
}

// Get the joystick values
BLYNK_WRITE(V0) {
  x = param[0].asInt();
}
// Get the joystick values
BLYNK_WRITE(V1) {
  y = param[0].asInt();
}
//Get the slider values
BLYNK_WRITE(V2) {
  Speed = param.asInt();
}

// Check these values using the IF condition
void smartcar() {
  if (y > 70) {
    carForward();
    Serial.println("carForward");
  } else if (y < 30) {
    carBackward();
    Serial.println("carBackward");
  } else if (x < 30) {
    carLeft();
    Serial.println("carLeft");
  } else if (x > 70) {
    carRight();
    Serial.println("carRight");
  } else if (x < 70 && x > 30 && y < 70 && y > 30) {
    carStop();
    Serial.println("carstop");
  }
}
void loop() {
  Blynk.run();// Run the blynk function
  timer.run();
  smartcar();// Call the main function
  
}

/*************Motor movement functions****************/
void carForward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carBackward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carLeft() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carRight() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
