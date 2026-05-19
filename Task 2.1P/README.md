# 🌡️💡 Room Conditions Monitor

An Arduino based IoT project that reads room temperature and light level
using a **DHT11 sensor** and a **Photodiode**, and uploads the data to
**ThingSpeak** every 30 seconds over WiFi.
This project was built to help automate and monitor the environmental
conditions of Linda's home, so that temperature and light levels can be
tracked remotely through a simple web interface.

---

## 📋 Project Overview

This project was built for **SIT210**. The Arduino Nano 33 IoT reads
temperature data from a DHT11 sensor and light level from a Photodiode,
and sends both to a ThingSpeak channel where Linda's carers can view them
as live charts on the web.

---

## 🛒 Hardware Required

| Component | Description |
|---|---|
| Arduino Nano 33 IoT | Microcontroller |
| DHT11 Sensor Module | Temperature and humidity sensor |
| Photodiode (FIT0180) | Analogue light sensor |
| 10k ohm Resistor | For photodiode voltage divider |
| Jumper Wires | For connections |
| USB Cable | To power and upload code |
| Breadboard | For circuit connections |

---

## 🔌 Pin Connections

### DHT11 Temperature Sensor
| DHT11 Pin | Arduino Pin |
|---|---|
| VCC | 3.3V |
| GND | GND |
| DATA | A1 |

### Photodiode (FIT0180)
| Photodiode | Arduino Pin |
|---|---|
| Short leg (cathode) | A0 + 10k ohm resistor to 3.3V |
| Long leg (anode) | GND |

> **Note:** The Arduino Nano 33 IoT runs on **3.3V logic only**.
> Do NOT connect to 5V.

---

## 📦 Libraries Required

Install these from **Arduino IDE -> Sketch -> Include Library -> Manage Libraries**:

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
4. Add **Field 2** and name it `Light Level`
5. Copy your **Write API Key** from the API Keys tab
6. Copy your **Channel ID**

---

## ⚙️ Configuration

Open the code and update these lines with your own details:

```cpp
const char* WIFI_SSID     = "Your_WiFi_Name";
const char* WIFI_PASSWORD = "Your_WiFi_Password";

unsigned long CHANNEL_ID    = YOUR_CHANNEL_ID;
const char*   WRITE_API_KEY = "YOUR_WRITE_API_KEY";
```

---

## 📊 Data

| Field | Sensor | Unit |
|---|---|---|
| Field 1 | DHT11 | Celsius (°C) |
| Field 2 | Photodiode | Percentage (%) |

Data is uploaded to ThingSpeak every **30 seconds**.
