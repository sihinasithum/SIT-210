# Task 5.1P - Making a Graphical User Interface

## Description

Linda was tired of using ordinary switches to control the lights around her house. She wanted something smarter and easier to use. This project solves that by building a simple **Python GUI** running on a **Raspberry Pi** that lets Linda switch on the lights in her **Living Room**, **Bathroom**, and **Closet** - all from one clean interface. Each room has a radio button, and clicking it turns on the matching LED while switching the others off. A dedicated Exit button safely shuts everything down.

## Hardware Required

- Raspberry Pi (any model with 40-pin GPIO header)
- 3x LEDs (any colour)
- 3x 330 ohm resistors
- Breadboard
- Jumper wires
- HDMI monitor, keyboard and mouse (to use the GUI on the Pi)

## Libraries / Dependencies Required

| Library / Package | Purpose | How to Install |
|---|---|---|
| `RPi.GPIO` | Controls the GPIO pins on the Raspberry Pi | Pre-installed on Raspberry Pi OS |
| `tkinter` | Builds the GUI window and widgets | Pre-installed with Python on Raspberry Pi OS |

> No extra installations needed - both libraries come built into Raspberry Pi OS.

## Pin Map

| Component | GPIO Number | Physical Pin |
|---|---|---|
| LED 1 - Living Room | GPIO 17 | Pin 11 |
| LED 2 - Bathroom | GPIO 27 | Pin 13 |
| LED 3 - Closet | GPIO 22 | Pin 15 |
| GND (shared, all LEDs) | GND | Pin 6, 9, or 14 |

> GPIO pins on the Raspberry Pi operate at **3.3V logic**. Always use a **330 ohm resistor** in series with each LED to limit current and prevent damage to the GPIO pin and the LED.

## How It Works

1. The Raspberry Pi is powered on and the Python script is launched in Thonny (or terminal).
2. GPIO pins 17, 27, and 22 are configured as **outputs** - ready to send power to the LEDs.
3. The tkinter GUI window opens on screen with three radio buttons (one per room) and an Exit button.
4. When Linda clicks a radio button (e.g. **Living Room**), the `turn_on()` function runs.
5. The function first **turns off all three LEDs** by setting all GPIO pins to `False`.
6. It then turns **only the selected LED on** by setting its GPIO pin to `True`.
7. This means only one light is ever on at a time - the others always go off first.
8. When Linda clicks **Exit** (or closes the window), `GPIO.cleanup()` resets all pins safely before the program closes.

## Code Overview

### `turn_on(pin)`
The main logic function. Takes a GPIO pin number as input. It first sets **all three GPIO pins to False** (off), then sets **only the given pin to True** (on). This ensures only one LED is active at any time.

```python
def turn_on(pin):
    GPIO.output(17, False)
    GPIO.output(27, False)
    GPIO.output(22, False)
    GPIO.output(pin, True)
```

### `exit_app()`
Called when the user clicks Exit or closes the window. Calls `GPIO.cleanup()` to safely reset all GPIO pins back to their default state, then destroys the tkinter window.

```python
def exit_app():
    GPIO.cleanup()
    window.destroy()
```

### GUI / Main Loop
The tkinter window is set up with a title label, three `Radiobutton` widgets (one per room), and a styled Exit button. Each radio button is linked to `turn_on()` via a `lambda`. The `window.protocol("WM_DELETE_WINDOW", exit_app)` line ensures GPIO is cleaned up properly whether the user clicks Exit or the window's X button. `window.mainloop()` keeps the GUI running and listening for button clicks.

## Configuration

If you want to use different GPIO pins (e.g. because some pins are already in use), update these lines at the top of the script:

```python
GPIO.setup(17, GPIO.OUT)   # change 17 to your pin for Living Room
GPIO.setup(27, GPIO.OUT)   # change 27 to your pin for Bathroom
GPIO.setup(22, GPIO.OUT)   # change 22 to your pin for Closet
```

Also update the matching pin numbers inside `turn_on()` and the `command=lambda` on each `Radiobutton` to match.

> Make sure any new pins you choose are **GPIO-capable** and not reserved for special functions (I2C, SPI, UART). Refer to the `pinout` command on the Pi terminal for a full map.

**Sihina Sithum K A**
*Deakin University - Software Engineering*
*April 2026*
