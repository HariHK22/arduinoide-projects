#define BLYNK_TEMPLATE_ID "TMPL3BgnZu93M"
#define BLYNK_TEMPLATE_NAME "BIO Medical Kit"
#include <Wire.h>
#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
const int led1 = D0;


char auth[] = "znpzC21r-iytNKAJIi69ThMqpSaoCVjK"; // You should get Auth Token in the Blynk App.
char ssid[] = "BIO";                               // Your WiFi credentials.
char pass[] = "aaaaaaaa";

const int MPU_addr = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

int minVal = 265;
int maxVal = 402;

double x;
double y;
double z;

void setup() {
  pinMode(led1,OUTPUT);
  Wire.begin(D5, D4); // Specify D3 for SDA and D4 for SCL
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
}


void loop() {
  Blynk.run();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  int xAng = map(AcX, minVal, maxVal, -90, 90);
  int yAng = map(AcY, minVal, maxVal, -90, 90);
  int zAng = map(AcZ, minVal, maxVal, -90, 90);

  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

  Serial.print("AngleX= ");
  Serial.println(x);

  Serial.print("AngleY= ");
  Serial.println(y);

  Serial.print("AngleZ= ");
  Serial.println(z);
  Serial.println("-----------------------------------------");

  Blynk.virtualWrite(V0, x);
  Blynk.virtualWrite(V1, y);
  Blynk.virtualWrite(V2, z);
  Serial.print("Raw AcX= ");
Serial.println(AcX);
Serial.print("Raw AcY= ");
Serial.println(AcY);
Serial.print("Raw AcZ= ");
Serial.println(AcZ);


  // Check if acceleration is high in all axes
  if (abs(xAng) > 300 && abs(yAng) > 300 && abs(zAng) > 300) {
    Blynk.virtualWrite(V3, "INPUT was abnormal");
    digitalWrite(led1,HIGH);
  } else {
    Blynk.virtualWrite(V3, "INPUT was normal");
    digitalWrite(led1,LOW);
  }

  delay(1000);
}
