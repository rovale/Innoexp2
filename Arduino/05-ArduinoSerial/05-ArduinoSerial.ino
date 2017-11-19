const int ledPin = 3;
const int lightPin = A0;

unsigned long previousMillis = 0;
const long interval = 500;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.setTimeout(100);
}

// the loop function runs over and over again forever
void loop() {
  if (Serial.available()) {
    int ledValue = Serial.parseInt();
    analogWrite(ledPin, ledValue);
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    int light = analogRead(lightPin);
    Serial.println(light);
  }  
}
