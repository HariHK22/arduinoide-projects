#define BLYNK_TEMPLATE_ID "TMPL39j1GiMyE"
#define BLYNK_TEMPLATE_NAME "Driver alert"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// WiFi credentials
char auth[] = "QF1EecZoZ3G9Aaf2vaBdWj2Ryel1CVss";
char ssid[] = "HK";
char pass[] = "aaaaaaaa";

// Pins
const int blinkPin = D2;    // Assuming the input pin is connected to D2 on the ESP8266
const int motorPin = D7;    // Output pin for the motor
const int buzzerPin = D6;   // Output pin for the buzzer

long currentTime = 0;       // Initialize currentTime
int buzzerState = HIGH;
unsigned long previousMillis = 0;
const long interval = 500;  // Interval in milliseconds for the buzzer to toggle

void setup() {
  pinMode(motorPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(blinkPin, INPUT);
  digitalWrite(motorPin, HIGH);  // Turn off motor initially
digitalWrite(buzzerPin, HIGH);  // Turn off motor initially
  // Connect to Wi-Fi
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
  
  if (!digitalRead(blinkPin)) {
    currentTime = millis();
    while (!digitalRead(blinkPin)) {
      if (TimeDelay() >= 3) {
         Blynk.virtualWrite(V1, 0);
         Blynk.virtualWrite(V2, "The driver had fallen asleep.");
        // Toggle buzzer state only when the condition is met
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
          // Save the last time the buzzer state was toggled
          previousMillis = currentMillis;

          // Toggle the buzzer state
          buzzerState = !buzzerState;
          digitalWrite(buzzerPin, buzzerState);

          if (TimeDelay() >= 4) {
            digitalWrite(motorPin, LOW);  // Turn on motor
          }   

                    if (TimeDelay() >= 10) {
           Blynk.logEvent("driver", "Emergency:The driver No. 12 had fallen asleep.");
          }  
        }
      }

      delay(1000);
    }
  } else {
     Blynk.virtualWrite(V1, 1);
     Blynk.virtualWrite(V2, "The driver wasn't asleep.");
    digitalWrite(buzzerPin, LOW);   // Turn off buzzer
    digitalWrite(motorPin, HIGH);    // Turn off motor
  }
}

int TimeDelay() {
  long t = millis() - currentTime;
  t = t / 1000;
  return t;
}