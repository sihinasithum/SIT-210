#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <DHT.h>

// ----------WiFi---------- 
const char* WIFI_SSID     = "Sihina’s iPhone";
const char* WIFI_PASSWORD = "12345678"; 

// --------- ThingSpeak ----------
unsigned long CHANNEL_ID    = 3300914;
const char*   WRITE_API_KEY = "D8PW7O973G2SETB2";

// -------- Sensor --------
#define DHT_PIN  A1
#define DHT_TYPE DHT11

DHT        dht(DHT_PIN, DHT_TYPE);
WiFiClient client;

// --------- Timer ------------
const unsigned long UPLOAD_INTERVAL = 30000; // 30 seconds
unsigned long lastUploadTime = 0;

// ---------Connect to WiFi ---------
void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".=");
  }
  Serial.println("\nWiFi connected!");
}

// -------------Setup DHT11 sensor-----------
void setupSensor() {
  dht.begin();
  Serial.println("DHT11 sensor ready.");
}


// -------Read temperature from DHT11---------
float readTemperature() {
  float temp = dht.readTemperature();
  if (isnan(temp)) {
    Serial.println("ERROR: DHT11 failed!");
    return -1;
  }
  return temp;
}


// -----------Upload temperature to ThingSpeak----------
void uploadData(float temperature) {
  ThingSpeak.setField(1, temperature); // Field 1 = Temperature

  int result = ThingSpeak.writeFields(CHANNEL_ID, WRITE_API_KEY);
  if (result == 200) {
    Serial.println("Upload successful!");
  } else {
    Serial.println("Upload failed. Code: " + String(result));
  }
}

//----------setup------------
void setup() {
  Serial.begin(9600);
  delay(1000);
  connectWiFi();
  ThingSpeak.begin(client);
  setupSensor();
}

//----------loop--------------
void loop() {
  unsigned long now = millis();

  if (now - lastUploadTime >= UPLOAD_INTERVAL) {
    float temperature = readTemperature();

    Serial.print("Temp: "); Serial.print(temperature); Serial.println(" C");

    uploadData(temperature);
    lastUploadTime = now;
  }
}
