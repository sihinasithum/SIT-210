# Task 1.1P - Switching ON Lights  

Linda arrives home from bingo sessions at the retirement village when it's dark. She presses one simple push button near her door, and the system automatically turns on the porch light for 30 seconds and hallway light for 60 seconds. The button triggers both lights at once so she can safely find her keys, unlock the door, and walk inside and lights turn off automatically to save power.

## 🖥️ **Hardware Setup**

```
Board: Arduino Nano 33 IoT 
Green LED + 220Ω 
Yellow LED + 220Ω  
Push Button
BreadBord
Jumper wires
```

## Code overview

### Pin setup
- Porch light: Green LED (PIN D9)
- Hall light: Yellow LED (PIN D8) 
- Button: (PIN D2)

### setup() function
The button is set as INPUT_PULLUP, 
which means the button reads HIGH normally and goes LOW when pressed. 
The two LED pins are set as outputs so we can turn them on and off.

### Functions
The code is divided into four dedicated functions,
- **turnOnLights()** — Turns both the porch (green) and hall (yellow) LEDs ON at the same time
- **porchSequence()** — Waits 30 seconds then turns the porch light OFF
- **hallSequence()** — Waits 30 seconds then turns the hall light OFF
- **waitForRelease()** — Waits until the button is released to prevent the sequence from triggering multiple times in a row


### loop() function
The loop() function constantly checks if the button is being pressed. When pressed, it calls each module in order to carry out the lighting sequence.

1. Both the porch light and hall light turn ON at the same time
2. The system waits 30 seconds
3. The porch light turns OFF
4. The system waits another 30 seconds
5. The hall light turns OFF
6. The code then waits until the button is released before it can be 
   pressed again - this stops it from triggering multiple times in a row
**OFF**


## 🖼️ **Demo**

![Demo](https://github.com/sihinasithum/SIT-210/blob/main/Task%201.1P/Assets/Copy%20of%20Arduino%20Nano%20(2).png)


***

**Sihina Sithum K A**  
*Deakin University - Software Engineering*  
*March 2026*

***
