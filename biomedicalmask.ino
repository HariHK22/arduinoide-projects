#define BLYNK_TEMPLATE_ID "TMPL3hOeqvIIs"
#define BLYNK_TEMPLATE_NAME "BIOmedical"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "a1mfsTkEzc1LL4T-OF9AkK_LelhsLDcE";  // Your Blynk auth token
char ssid[] = "HK"; // Your WiFi network name
char pass[] = "aaaaaaaa";    // Your WiFi password

#define IR_SENSOR_PIN_1 D2 // Connect first IR sensor to digital pin D2
#define IR_SENSOR_PIN_2 D5 // Connect second IR sensor to digital pin D3
#define RELAY_PIN D4        // Connect relay module to digital pin D4

BlynkTimer timer;

void setup() {
  Serial.begin(9600);
  delay(10);

  Blynk.begin(auth, ssid, pass);
  Serial.println("Connecting to Blynk...");
  while (!Blynk.connected()) {
    Blynk.connect();
  }
  Serial.println("Connected to Blynk.");

  pinMode(IR_SENSOR_PIN_1, INPUT);
  pinMode(IR_SENSOR_PIN_2, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  timer.setInterval(100L, checkIRSensors);
}

void loop() {
  Blynk.run();
  timer.run();
}

void checkIRSensors() {
  int irSensor1State = digitalRead(IR_SENSOR_PIN_1);
  int irSensor2State = digitalRead(IR_SENSOR_PIN_2);

  // Update virtual pin for sensor 1
  Blynk.virtualWrite(V1, irSensor1State);

  if (irSensor1State == HIGH) {
    // If IR sensor 1 detects something, trigger relay
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, LOW);
  }

  if (irSensor2State == LOW) {
    // If IR sensor 2 detects something, send notification
    Serial.println("Patient number x needs assistance.");
    Blynk.logEvent("alert", "Patient number x needs assistance.");
  }
}
