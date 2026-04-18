# 🚨 Task 4.1P - Handling Interrupts

## 📋 Description

Linda is back from her Bingo nights at the retirement village  but finding the light switch in the dark is just as tricky as finding her keys. This system solves that automatically. When Linda approaches the door at night, a PIR motion sensor detects her presence and instantly switches ON both the porch and hallway lights  no button pressing needed. If for any reason the motion sensor misses her, a slider switch mounted near the door gives her a reliable backup to toggle the lights manually. Both triggers are handled using **hardware interrupts**, making the system highly responsive without wasting time constantly checking sensor states in the loop.

---

## 🛒 Hardware Required

- Arduino Nano 33 IoT
- PIR motion sensor (HC-SR501)
- BH1750 light sensor
- Slider switch (3-pin)
- Two LEDs (any colour)
- Two 220Ω resistors
- Breadboard and jumper wires

---

## 📦 Libraries Required

Install these from **Arduino IDE > Tools > Manage Libraries**:

| Library | Author |
|---|---|
| `Wire` | Arduino (built-in) |
| `BH1750` | Christopher Laws |

---

## 🔌 Pin Map

| Component | Component Pin | Arduino Nano 33 IoT |
|---|---|---|
| PIR Sensor | VCC | Vin |
| PIR Sensor | OUT | D2 |
| PIR Sensor | GND | GND |
| Slider Switch | COM (middle pin) | GND |
| Slider Switch | Outer pin | D3 |
| LED 1 | + (long leg) | D4 |
| LED 1 | – (short leg) | GND via 220Ω |
| LED 2 | + (long leg) | D5 |
| LED 2 | – (short leg) | GND via 220Ω |
| BH1750 | VCC | 3.3V |
| BH1750 | GND | GND |
| BH1750 | SDA | A4 |
| BH1750 | SCL | A5 |
| BH1750 | ADDR | GND |

> **Note:** The Arduino Nano 33 IoT runs on **3.3V logic only**. Do NOT connect BH1750 VCC to 5V. Use the PIR sensor on Vin (5V) as it requires at least 4.5V to operate correctly.

---

## 💡 How It Works

1. On startup the Arduino initialises the BH1750 light sensor and attaches two hardware interrupts  one on the PIR pin (D2) and one on the slider switch pin (D3)
2. When Linda approaches the door, the PIR sensor triggers the motion interrupt (`RISING` edge)
3. The interrupt sets a `motionFlag`  the main loop then reads the current lux level from the BH1750
4. If it is dark (below 50 lux), both LEDs switch ON and the Serial Monitor prints a confirmation message
5. If the lights are already ON when motion is detected, the system skips the lux check and simply prints that the lights are already on  no unnecessary toggling
6. If it is bright outside (above 50 lux), the lights stay OFF and the Serial Monitor prints a "too bright" message
7. At any time, sliding the switch triggers the switch interrupt (`CHANGE` edge) which toggles both LEDs regardless of the lux level  giving Linda full manual backup control
8. The Serial Monitor prints a message for every state change so the system behaviour can be monitored and verified

---

## 💻 Code Overview

### Pin setup
- PIR sensor signal: D2 (interrupt, RISING)
- Slider switch: D3 (interrupt, CHANGE  INPUT_PULLUP, no resistor needed)
- LED 1: D4 (output)
- LED 2: D5 (output)
- BH1750: SDA -> A4, SCL -> A5

### ISR functions
Two minimal Interrupt Service Routines are used  they only set a flag and return immediately. This is the correct and safe way to use interrupts on Arduino, as ISRs must be as short as possible:
- **`onMotion()`**  sets `motionFlag = true` when PIR detects movement
- **`onSwitch()`**  sets `switchFlag = true` when the slider switch changes state

### `setup()` function
Initialises Serial, Wire, and the BH1750 sensor. Configures all pin modes and attaches both interrupts using `attachInterrupt()` with `digitalPinToInterrupt()` for safe pin mapping on the SAMD21.

### `setLEDs()` function
A shared helper that sets both LEDs to the same state and updates the `ledsOn` boolean. Called by both the motion and switch logic to keep LED control consistent and avoid code duplication.

### `loop()` function
The main loop checks the two flags set by the ISRs and handles the actual logic:

1. If `motionFlag` is set -> check if lights are already ON -> if not, read lux -> switch ON if dark
2. If `switchFlag` is set -> toggle both LEDs to the opposite of their current state

***

**Sihina Sithum K A**
*Deakin University - Software Engineering*
*April 2026*

***
