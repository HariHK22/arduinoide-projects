#include <Servo.h>

Servo servo1;
Servo servo2;

int moisturePin = A0;
int moistureValue;
int servoAngle1;
int servoAngle2;

void setup() {
  servo1.attach(9);  // Attach servo1 to pin 9
  servo2.attach(11); // Attach servo2 to pin 11
}

void loop() {
  moistureValue = analogRead(moisturePin);
  servoAngle1 = map(moistureValue, 0, 1023, 10, 180);
  servoAngle2 =map(moistureValue, 0, 1023, 10, 180);

  servo1.write(servoAngle1);
  servo2.write(servoAngle2);
  delay(1000);  // Adjust the delay as needed
}
