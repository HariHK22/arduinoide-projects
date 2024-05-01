#define BLYNK_TEMPLATE_ID "TMPL3iOLfxyjv"
#define BLYNK_TEMPLATE_NAME "sos"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

BlynkTimer timer;

const int IR_PIN3 = D2;

char auth[] = "OBDdeXdQh_KkogIv_XZADnBjWekKZihR";
char ssid[] = "SOS";
char pass[] = "aaaaaaaa";

bool objectDetected3 = false;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(IR_PIN3, INPUT);
}

void loop() {
  Blynk.run();
  int sensorValue3 = digitalRead(IR_PIN3);
  Serial.print("Sensor Value 3: ");
  Serial.println(sensorValue3);

  if (sensorValue3 == LOW && !objectDetected3) {
    Serial.println("Object Detected on Sensor 3");
    Blynk.logEvent("sos", "Emergency: Patient No.1 needs immediate attention!");
    objectDetected3 = true;
  }

  // If no object detected by sensor 3, update label value and reset flag
  if (sensorValue3 == HIGH && objectDetected3) {
    Serial.println("Object Not Detected on Sensor 3");
    objectDetected3 = false;
  }
}
