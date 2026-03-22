const int porchPin = 9; // green (porch)
const int hallPin = 8; // yellow (hall)
const int buttonPin = 2; // button

void setup() {
    pinMode(porchPin, OUTPUT);
    pinMode(hallPin, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);
}

//turn both lights ON
void turnOnLights() {
    digitalWrite(porchPin, HIGH);
    digitalWrite(hallPin, HIGH);
}

//porch light timing 
void porchSequence() {
    delay(30000);
    digitalWrite(porchPin, LOW);
}

//hall light timing 
void hallSequence() {
    delay(30000);
    digitalWrite(hallPin, LOW);
}

//wait for button release
void waitForRelease() {
    while(digitalRead(buttonPin) == LOW);
}


void loop() {
    if (digitalRead(buttonPin) == LOW) {
        turnOnLights();
        porchSequence();
        hallSequence();
        waitForRelease();
    }
}
