// =============================================
// Smart Adaptive Fan Controller — Linda's Room
// Arduino Nano 33 IoT
// =============================================

#include <DHT.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <WiFiUDP.h>
#include <NTPClient.h>

// --- Pin definitions ---
#define DHT_PIN       4
#define FAN_PIN       5
#define SWITCH_PIN    3
#define PIR_PIN       2
#define DHT_TYPE      DHT11

// --- Demo-ready constants ---
#define TEMP_FAN_START      20.0
#define TEMP_FAN_MAX        30.0
#define FAN_MIN_PWM         100
#define FAN_MAX_PWM         255
#define NO_MOTION_TIMEOUT   30000    // 30 sec for demo (change to 300000 for production)
#define NIGHT_START         22
#define NIGHT_END           7
#define PRINT_INTERVAL      2000
#define MQTT_INTERVAL       5000

// --- WiFi credentials ---
const char* ssid     = "Sihina's iPhone";
const char* password = "12345678";

// --- MQTT broker ---
const char* broker = "broker.emqx.io";
const int   port   = 1883;

// --- MQTT topics ---
const char* TOPIC_TEMP      = "linda/fan/temperature";
const char* TOPIC_MOTION    = "linda/fan/motion";
const char* TOPIC_SWITCH    = "linda/fan/switch_state";
const char* TOPIC_SPEED     = "linda/fan/speed_pct";
const char* TOPIC_TIME_MODE = "linda/fan/time_mode";
const char* TOPIC_CMD_MODE  = "linda/fan/cmd/mode";
const char* TOPIC_CMD_SPEED = "linda/fan/cmd/manual_speed";

// --- Objects ---
DHT dht(DHT_PIN, DHT_TYPE);
WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 36000); // UTC+10 AEST

// --- State variables ---
volatile unsigned long lastMotionTime = 0;
unsigned long lastPrintTime  = 0;
unsigned long lastMQTTTime   = 0;
String carerMode    = "AUTO";
int    carerSpeedPct = 0;
float  temperature  = 0;
int    fanSpeed     = 0;

// --- PIR interrupt ---
void onMotion() {
  lastMotionTime = millis();
}

// --- MQTT incoming command handler ---
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) msg += (char)payload[i];

  if (String(topic) == TOPIC_CMD_MODE) {
    carerMode = msg;
  } else if (String(topic) == TOPIC_CMD_SPEED) {
    carerSpeedPct = msg.toInt();
  }
}

// --- WiFi connection ---
void connectWiFi() {
  Serial.print("Connecting to WiFi");
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" Connected!");
}

// --- MQTT connection ---
void connectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("Connecting to MQTT...");
    if (mqtt.connect("LindaFanController")) {
      Serial.println("Connected!");
      mqtt.subscribe(TOPIC_CMD_MODE);
      mqtt.subscribe(TOPIC_CMD_SPEED);
    } else {
      Serial.print("Failed rc=");
      Serial.println(mqtt.state());
      delay(2000);
    }
  }
}

// --- Fan speed calculation from temperature ---
int calcFanSpeed(float temp) {
  if (temp < TEMP_FAN_START) return 0;
  if (temp >= TEMP_FAN_MAX)  return FAN_MAX_PWM;
  float ratio = (temp - TEMP_FAN_START) / (TEMP_FAN_MAX - TEMP_FAN_START);
  return FAN_MIN_PWM + ratio * (FAN_MAX_PWM - FAN_MIN_PWM);
}

// --- Night time check ---
bool isNight(int hour) {
  return (hour >= NIGHT_START || hour < NIGHT_END);
}

void setup() {
  Serial.begin(9600);

  // Pin setup
  pinMode(FAN_PIN,    OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(PIR_PIN,    INPUT);
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), onMotion, RISING);

  dht.begin();
  connectWiFi();

  timeClient.begin();
  timeClient.update();

  mqtt.setServer(broker, port);
  mqtt.setCallback(mqttCallback);
  connectMQTT();

  Serial.println("System ready.\n");
}

void loop() {
  // Keep connections alive
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop();
  timeClient.update();

  // Read sensors and switch
  temperature    = dht.readTemperature();
  bool switchON  = (digitalRead(SWITCH_PIN) == LOW);
  int  hour      = timeClient.getHours();
  bool night     = isNight(hour);

  // Occupancy — PIR timeout only applies during the day
  unsigned long timeSinceMotion = millis() - lastMotionTime;
  bool occupied = night || (timeSinceMotion < NO_MOTION_TIMEOUT);

  // Fan control — carer mode always wins
  if (carerMode == "OFF") {
    fanSpeed = 0;

  } else if (carerMode == "MANUAL") {
    fanSpeed = map(carerSpeedPct, 0, 100, 0, 255);

  } else { // AUTO
    if (!switchON) {
      fanSpeed = 0;
    } else if (!night && !occupied) {
      fanSpeed = 0;
    } else {
      fanSpeed = calcFanSpeed(temperature);
    }
  }

  // Apply fan speed
  analogWrite(FAN_PIN, fanSpeed);

  // Print to terminal every 2 seconds
  if (millis() - lastPrintTime >= PRINT_INTERVAL) {
    lastPrintTime = millis();
    int speedPct  = map(fanSpeed, 0, 255, 0, 100);

    Serial.println("=============================");
    Serial.print("Temp:          "); Serial.print(temperature);             Serial.println(" C");
    Serial.print("Fan Speed:     "); Serial.print(speedPct);                Serial.println(" %");
    Serial.print("Switch:        "); Serial.println(switchON  ? "ON"       : "OFF");
    Serial.print("Last Motion:   "); Serial.print(timeSinceMotion / 1000);  Serial.println(" sec ago");
    Serial.print("Occupancy:     "); Serial.println(occupied   ? "OCCUPIED" : "EMPTY");
    Serial.print("Time Mode:     "); Serial.println(night      ? "NIGHT"    : "DAY");
    Serial.print("Carer Mode:    "); Serial.println(carerMode);
    Serial.println("=============================\n");
  }

  // Publish to MQTT every 5 seconds
  if (millis() - lastMQTTTime >= MQTT_INTERVAL) {
    lastMQTTTime  = millis();
    int speedPct  = map(fanSpeed, 0, 255, 0, 100);

    mqtt.publish(TOPIC_TEMP,      String(temperature).c_str());
    mqtt.publish(TOPIC_MOTION,    occupied  ? "1"     : "0");
    mqtt.publish(TOPIC_SWITCH,    switchON  ? "ON"    : "OFF");
    mqtt.publish(TOPIC_SPEED,     String(speedPct).c_str());
    mqtt.publish(TOPIC_TIME_MODE, night     ? "NIGHT" : "DAY");
  }
}
