const int ledPin = 3;
const int buttonPin = 2;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

// the loop function runs over and over again forever
void loop() {
  if (digitalRead(buttonPin) == LOW){
    digitalWrite(ledPin, HIGH);
  }
  else {
    digitalWrite(ledPin, LOW);
  }
}
