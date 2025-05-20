#include <WiFi.h>
#include <HTTPClient.h>
#include <HX711.h>
#include <driver/dac.h>

// Pin Definitions
#define TARE_BUTTON_PIN 18
#define ANNOUNCE_BUTTON_PIN 19
#define LOADCELL_DOUT_PIN 15
#define LOADCELL_SCK_PIN 14

// Wi-Fi Credentials
const char* ssid = "HK";
const char* password = "aaaaaaaa";

// HX711 Load Cell
HX711 scale;
float calibration_factor = 1100.0;  // Adjust this for your load cell

void setup() {
  Serial.begin(115200);

  // Initialize buttons
  pinMode(TARE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ANNOUNCE_BUTTON_PIN, INPUT_PULLUP);

  // Initialize HX711
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor);
  scale.tare();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");

  // Enable DAC output
  dac_output_enable(DAC_CHANNEL_1);  // GPIO25
}

void loop() {
  // Handle tare button
  if (digitalRead(TARE_BUTTON_PIN) == LOW) {
    scale.tare();
    speak("Scale reset to zero.");
    delay(500);  // Debounce delay
  }

  // Handle announce button
  if (digitalRead(ANNOUNCE_BUTTON_PIN) == LOW) {
    long weight = getWeight();
    speak("The weight is " + String(weight) + " grams.");
    delay(500);  // Debounce delay
  }

  delay(100);
}

long getWeight() {
  return scale.get_units(10);  // Average of 10 readings
}

void speak(String text) {
  HTTPClient http;
  String tts_url = "https://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q=" + text + "&tl=en";

  Serial.println("Connecting to TTS API...");
  http.begin(tts_url);
  int httpCode = http.GET();

  Serial.print("HTTP Response Code: ");
  Serial.println(httpCode);

  if (httpCode == HTTP_CODE_OK) {
    Serial.println("Connected to TTS API!");
    WiFiClient *stream = http.getStreamPtr();
    while (stream->available()) {
      uint8_t audioData = stream->read();
      dac_output_voltage(DAC_CHANNEL_1, audioData);
      delayMicroseconds(100);
    }
  } else {
    Serial.println("Error: Unable to connect to TTS API.");
  }

  http.end();
}

