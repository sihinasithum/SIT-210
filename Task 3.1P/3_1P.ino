#include <WiFiNINA.h>
#include <BH1750.h>
#include <Wire.h>

//WiFi credentials 
char ssid[] = "Sihina’s iPhone";
char pass[] = "12345678";

WiFiClient client;

char HOST_NAME[] = "maker.ifttt.com";
String KEY = "0acW02pK5fQetkMNpSfjN"; // IFTTT Webhooks key
float THRESHOLD = 500.0;           // Lux level to decide if it is sunny 
bool lastState = false;            // false = dark, true = sunny

BH1750 lightMeter;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Wire.begin();
  lightMeter.begin();
  Serial.println("BH1750 ready");

  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
  } else {
    Serial.println("\nWiFi connection failed");
  }
}

void loop() {
 float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");

  // Sunlight just STARTED
  if (lux >= THRESHOLD && lastState == false) {
    lastState = true;
    Serial.println("Sunlight started! Sending notification...");

    String PATH_NAME = "/trigger/sunlight_started/with/key/" + KEY;
    String queryString = "?value1=" + String(lux);

    if (client.connect(HOST_NAME, 80)) {
      Serial.println("Connected to server");
      client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
      client.println("Host: " + String(HOST_NAME));
      client.println("Connection: close");
      client.println();

      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          Serial.print(c);
        }
      }
      client.stop();
      Serial.println("disconnected");
    } else {
      Serial.println("connection failed");
    }
  }

  // Sunlight just STOPPED
  if (lux < THRESHOLD && lastState == true) {
    lastState = false;
    Serial.println("Sunlight stopped! Sending notification...");

    String PATH_NAME = "/trigger/sunlight_stopped/with/key/" + KEY;
    String queryString = "?value1=" + String(lux);

    if (client.connect(HOST_NAME, 80)) {
      Serial.println("Connected to server");
      client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
      client.println("Host: " + String(HOST_NAME));
      client.println("Connection: close");
      client.println();

      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          Serial.print(c);
        }
      }
      client.stop();
      Serial.println("disconnected");
    } else {
      Serial.println("connection failed");
    }
  }

  delay(5000); // Check the light every 5 seconds
}
