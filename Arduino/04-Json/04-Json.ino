/*
Sends the light sensor value every 500 seconds:
{"light": <0-1024>}

Sets the dim value of the LED after receiving:
{dimmer: <0-255>}

Documentation of the Json library:
https://arduinojson.org/
*/

#include <ArduinoJson.h>

const int ledPin = 3;
const int lightPin = A0;

unsigned long previousMillis = 0;
const long interval = 500;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.setTimeout(100);
}

void loop() {
  if (Serial.available()) {
    DynamicJsonBuffer jsonBuffer(1024);
    JsonObject& root = jsonBuffer.parseObject(Serial);
    if (root.success()) {
      int ledValue = root["dimmer"];
      analogWrite(ledPin, ledValue);
    }
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    int light = analogRead(lightPin);
    
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["light"] = light;
    root.printTo(Serial);
  }
}
