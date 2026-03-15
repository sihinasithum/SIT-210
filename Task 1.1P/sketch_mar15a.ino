const int porchPin = 9;  // D9 green (porch)
const int hallPin = 8;   // D8 yellow (hall)
const int buttonPin = 2; // D2 button

void setup() {
  pinMode(porchPin, OUTPUT);
  pinMode(hallPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    digitalWrite(porchPin, HIGH);  // BOTH ON
    digitalWrite(hallPin, HIGH);
    delay(30000);                  // Wait 30s (porch time)
    digitalWrite(porchPin, LOW);   // Porch OFF after 30s
    delay(30000);                  // Wait another 30s
    digitalWrite(hallPin, LOW);    // Hall OFF after total 60s
    while(digitalRead(buttonPin) == LOW);  // Wait release
  }
}
