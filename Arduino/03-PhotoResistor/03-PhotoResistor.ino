const int lightPin = A0;

unsigned long previousMillis = 0;
const long interval = 500;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  int light = analogRead(lightPin);

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.println(light);
  }
}
