# ☀️ Task 3.1P - Create Notification based on Sensor Data

## 📋 Description
This project uses an Arduino Nano 33 IoT and a BH1750 light sensor to monitor 
sunlight exposure for a terrarium. When sunlight starts or stops hitting the 
terrarium, the Arduino sends a notification via IFTTT Webhooks to trigger an 
email alert.

## 🛒 Hardware Required
- Arduino Nano 33 IoT
- BH1750 Light Sensor
- Breadboard and jumper wires

---

## 📦 Libraries Required

Install these from **Arduino IDE > Tools > Manage Libraries**:

| Library | Author |
|---|---|
| `WiFiNINA` | Arduino |
| `BH1750` | Christopher Laws |

---

## 🔌 Pin Map
| BH1750 Pin | Arduino Nano 33 IoT |
|------------|---------------------|
| VCC        | 3.3V                |
| GND        | GND                 |
| SDA        | A4                  |
| SCL        | A5                  |
| ADDR       | GND                 |


> **Note:** The Arduino Nano 33 IoT runs on **3.3V logic only**. Do NOT connect to 5V.

---

## 💡 How It Works
1. The Arduino connects to WiFi on startup
2. Every 5 seconds it reads the light level in lux from the BH1750 sensor
3. If light goes above 500 lux (sunlight started), it triggers the IFTTT 
   `sunlight_started` event and sends an email notification
4. If light drops below 500 lux (sunlight stopped), it triggers the IFTTT 
   `sunlight_stopped` event and sends another email notification
5. A boolean `lastState` prevents repeated triggers - it only fires once 
   when the state changes

## 📱 IFTTT Setup
1. Create a free account at [ifttt.com](https://ifttt.com)
2. Create a new applet - click **Create**
3. Choose **Webhooks** as the trigger -> **Receive a web request**
4. Set Event Name to `sunlight_started` -> Create trigger
5. Choose **Email** as the action -> Create action
6. Repeat steps 2-5 for a second applet with Event Name `sunlight_stopped`
7. Go to **Webhooks -> Documentation** to copy your personal API key

## ⚙️ Configuration
Open the code and update these lines with your own details:

```cpp
char ssid[] = "Your_WiFi_Name";
char pass[] = "Your_WiFi_Password";

String KEY      = "YOUR_IFTTT_KEY";
float THRESHOLD = 500.0;        // Adjust based on your environment
```
---

***

**Sihina Sithum K A**  
*Deakin University - Software Engineering*  
*March 2026*

***


