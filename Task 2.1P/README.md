# 🌡️ Room Temperature Monitor

An Arduino-based IoT project that reads room temperature using a **DHT11 sensor** and uploads the data to **ThingSpeak** every 30 seconds over WiFi.
This project was built to help automate and monitor the environmental conditions of Linda's  home, so that temperature levels can be tracked remotely through a simple web interface.

---

## 📋 Project Overview

This project was built for **SIT210**. The Arduino Nano 33 IoT reads temperature data from a DHT11 sensor and sends it to a ThingSpeak channel where Linda's carers can view it as a live chart on the web.

---

## 🛒 Hardware Required

| Component | Description |
|---|---|
| Arduino Nano 33 IoT | Microcontroller with built-in WiFi |
| DHT11 Sensor Module | Temperature and humidity sensor |
| Jumper Wires | For connections |
| USB Cable | To power and upload code |
| bread bord|

---

## 🔌 Pin Connection

| DHT11  Pin | Arduino Pin |
|---|---|
| VCC | 3.3V |
| GND | GND |
| DATA | A1 |

>  **Note:** The Arduino Nano 33 IoT runs on **3.3V logic only**. Do NOT connect to 5V.

---
## 🔧 Demo
![demo](https://github.com/sihinasithum/SIT-210/blob/main/Task%202.1P/Assets/Copy%20of%20Arduino%20Nano%20Temp.png)

---
## 📦 Libraries Required

Install these from **Arduino IDE > Sketch > Include Library > Manage Libraries**:

| Library | Author |
|---|---|
| `DHT sensor library` | Adafruit |
| `Adafruit Unified Sensor` | Adafruit |
| `ThingSpeak` | MathWorks |
| `WiFiNINA` | Arduino |

---

## ☁️ ThingSpeak Setup

1. Create a free account at [thingspeak.com](https://thingspeak.com)
2. Create a new channel called **RoomConditions**
3. Add **Field 1** and name it `Temperature`
4. Copy your **Write API Key** from the API Keys tab
5. Copy your **Channel ID**

---

## ⚙️ Configuration

Open the code and update these lines with your own details:

```cpp
const char* WIFI_SSID     = "Your_WiFi_Name";
const char* WIFI_PASSWORD = "Your_WiFi_Password";

unsigned long CHANNEL_ID    = YOUR_CHANNEL_ID;
const char*   WRITE_API_KEY = "YOUR_WRITE_API_KEY";

