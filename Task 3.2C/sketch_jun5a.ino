#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>


char ssid[] = "Sihina's iPhone";
char pass[] = "12345678";

// MQTT broker
const char broker[] = "broker.emqx.io";
int port = 1883;


const int trigPin = 2;
const int echoPin = 3;
const int led1 = 4;   // hallway
const int led2 = 5;   // bathroom

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    delay(2000);
  }
  Serial.println("WiFi connected");

  while (!mqttClient.connect(broker, port)) {
    Serial.println("Connecting to broker...");
    delay(2000);
  }
  Serial.println("Broker connected");

  // Subscribe topics
  mqttClient.subscribe("ES/Wave");
  mqttClient.subscribe("ES/Pat");

  
  mqttClient.onMessage(onMqttMessage);
}

void loop() {
  mqttClient.poll();   

  long distance = readDistance();

  
  if (distance > 0 && distance < 10) {
    Serial.println("Pat detected");
    mqttClient.beginMessage("ES/Pat");
    mqttClient.print("sihina");
    mqttClient.endMessage();
    delay(1000);   
  }
  else if (distance >= 10 && distance < 20) {
    Serial.println("Wave detected");
    mqttClient.beginMessage("ES/Wave");
    mqttClient.print("sihina");
    mqttClient.endMessage();
    delay(1000);
  }
}

// Reads distance from  ultrasonic 
long readDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long cm = duration * 0.034 / 2;
  return cm;
}

// Runs automatically  message arrives
void onMqttMessage(int messageSize) {
  String topic = mqttClient.messageTopic();
  String message = "";
  while (mqttClient.available()) {
    message += (char)mqttClient.read();
  }

  Serial.print("Message on ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(message);

  if (topic == "ES/Wave") {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);   
  }
  else if (topic == "ES/Pat") {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);    
  }
}