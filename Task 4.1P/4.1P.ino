#include <Wire.h>
#include <BH1750.h>

const int PIR_PIN    = 2;
const int SWITCH_PIN = 3;
const int LED1_PIN   = 4;
const int LED2_PIN   = 5;

BH1750 lightMeter;

volatile bool motionFlag = false;
volatile bool switchFlag = false;
bool ledsOn = false;
bool led1On = false;
bool led2On = false;
unsigned long lightsOnTime = 0;

void onMotion() { motionFlag = true; }
void onSwitch() { switchFlag = true; }

void setLEDs(bool state) {
  ledsOn = state;
  led1On = state;
  led2On = state;
  digitalWrite(LED1_PIN, state);
  digitalWrite(LED2_PIN, state);
  if (state) lightsOnTime = millis();
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lightMeter.begin();

  pinMode(PIR_PIN, INPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(PIR_PIN), onMotion, RISING);
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), onSwitch, CHANGE);
}

void loop() {
  if (motionFlag) {
    motionFlag = false;

    if (ledsOn) {
      Serial.println("Motion detected — lights already ON");
    } else {
      float lux = lightMeter.readLightLevel();
      if (lux < 50) {
        setLEDs(true);
        Serial.println("Motion detected — lights ON (dark)");
      } else {
        Serial.println("Motion detected — too bright, lights stayed OFF");
      }
    }
  }

  if (switchFlag) {
    switchFlag = false;
    setLEDs(!ledsOn);
    Serial.println(ledsOn ? "Switch — lights ON" : "Switch — lights OFF");
  }

  // Auto-off timer
  if (ledsOn) {
    unsigned long elapsed = millis() - lightsOnTime;
    if (elapsed >= 30000 && led1On) {
      digitalWrite(LED1_PIN, LOW);
      led1On = false;
      Serial.println("Porch light OFF (30s)");
    }
    if (elapsed >= 60000 && led2On) {
      digitalWrite(LED2_PIN, LOW);
      led2On = false;
      ledsOn = false;
      Serial.println("Hallway light OFF (60s)");
    }
  }
}