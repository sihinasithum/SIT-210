#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <DHT.h>

// ---------- WiFi ----------
const char* WIFI_SSID     = "Sihina's iPhone";
const char* WIFI_PASSWORD = "12345678";

// ---------- ThingSpeak ----------
unsigned long CHANNEL_ID    = 3300914;
const char*   WRITE_API_KEY = "D8PW7O973G2SETB2";

// ---------- Sensors ----------
#define DHT_PIN   A1
#define DHT_TYPE  DHT11
#define LIGHT_PIN A0

DHT        dht(DHT_PIN, DHT_TYPE);
WiFiClient client;

// ---------- Timer ----------
const unsigned long UPLOAD_INTERVAL = 30000;
unsigned long lastUploadTime = 0;

// ---------- Connect to WiFi ----------
void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
}

// ---------- Setup Sensors ----------
void setupSensors() {
  dht.begin();
  Serial.println("DHT11 ready.");
  Serial.println("Light sensor ready.");
}

// ---------- Read Temperature ----------
float readTemperature() {
  float temp = dht.readTemperature();
  if (isnan(temp)) {
    Serial.println("ERROR: DHT11 failed!");
    return -1;
  }
  return temp;
}

// ---------- Read Light ----------
int readLight() {
  int raw = analogRead(LIGHT_PIN);
  int percentage = map(raw, 1023, 0, 0, 100);
  return percentage;
}

// ---------- Upload to ThingSpeak ----------
void uploadData(float temperature, int light) {
  ThingSpeak.setField(1, temperature); // Field 1 = Temperature
  ThingSpeak.setField(2, light);       // Field 2 = Light %

  int result = ThingSpeak.writeFields(CHANNEL_ID, WRITE_API_KEY);
  if (result == 200) {
    Serial.println("Upload successful!");
  } else {
    Serial.println("Upload failed. Code: " + String(result));
  }
}

// ---------- Setup ----------
void setup() {
  Serial.begin(9600);
  delay(1000);
  connectWiFi();
  ThingSpeak.begin(client);
  setupSensors();
}

// ---------- Loop ----------
void loop() {
  unsigned long now = millis();

  if (now - lastUploadTime >= UPLOAD_INTERVAL) {
    float temperature = readTemperature();
    int   light       = readLight();

    Serial.print("Temp: ");   Serial.print(temperature); Serial.println(" C");
    Serial.print("Light: ");  Serial.print(light);       Serial.println(" %");

    uploadData(temperature, light);
    lastUploadTime = now;
  }
}