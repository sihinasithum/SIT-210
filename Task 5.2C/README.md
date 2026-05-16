# Task 5.2C - Change the Light Intensity Using GUI

## Description

Linda was happy with the light controller built in Task 5.1P  but she had one more request. She wanted to be able to **dim and brighten** the light in her living room, not just switch it on and off. This project extends the Task 5.1P GUI by adding a **slider** to the Living Room that controls light intensity using **PWM (Pulse Width Modulation)**. The Bathroom and Closet remain as simple on/off checkboxes, exactly as before.

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
| `RPi.GPIO` | Controls GPIO pins and PWM on the Raspberry Pi | Pre-installed on Raspberry Pi OS |
| `tkinter` | Builds the GUI window and widgets | Pre-installed with Python on Raspberry Pi OS |

> No extra installations needed  both libraries come built into Raspberry Pi OS.

## Pin Map

| Component | GPIO Number | Physical Pin | Mode |
|---|---|---|---|
| LED 1 - Living Room | GPIO 18 | Pin 12 | PWM output |
| LED 2 - Bathroom | GPIO 27 | Pin 13 | Digital output |
| LED 3 - Closet | GPIO 22 | Pin 15 | Digital output |
| GND (shared, all LEDs) | GND | Pin 6, 9, or 14 | — |

> GPIO 18 is used for the Living Room because it supports **hardware PWM**, which allows smooth and precise brightness control. Always use a **330 ohm resistor** in series with each LED to limit current and protect the GPIO pin.

## What is PWM?

PWM stands for **Pulse Width Modulation**. Instead of sending a constant HIGH (3.3V) signal to the LED, the Pi rapidly switches the pin ON and OFF many times per second. The **duty cycle** controls how long the pin stays ON in each cycle:

- **0% duty cycle** -> LED is off
- **50% duty cycle** -> LED appears at half brightness
- **100% duty cycle** -> LED is fully on

In this project, PWM runs at **1000 Hz** and the slider adjusts the duty cycle from 0 to 100.

## How It Works

1. The Raspberry Pi is powered on and the Python script is launched in Thonny (or terminal).
2. GPIO 18 is set up as a PWM output at 1000 Hz, starting at 0% duty cycle (off).
3. GPIO 27 and GPIO 22 are set up as standard digital outputs same as Task 5.1P.
4. The tkinter GUI opens showing three room controls and an Exit button.
5. When Linda checks **Living Room**, `toggle_living()` runs and sets the PWM duty cycle to whatever the slider is currently showing.
6. Moving the slider calls `change_intensity()`, which updates the duty cycle in real time the LED visibly dims or brightens.
7. If Linda unchecks Living Room, the duty cycle is set to 0 and the LED turns off the slider position is remembered for next time.
8. **Bathroom** and **Closet** work exactly as in Task 5.1P checkboxes directly set the GPIO pin HIGH or LOW.
9. Clicking **Exit** (or closing the window) stops the PWM, runs `GPIO.cleanup()`, and closes the application safely.

## Code Overview

### `toggle_living()`
Called when the Living Room checkbox is clicked. If checked, it reads the current slider value and applies it as the PWM duty cycle. If unchecked, it sets the duty cycle to 0 (LED off).

```python
def toggle_living():
    if living_var.get():
        living_pwm.ChangeDutyCycle(intensity_slider.get())
    else:
        living_pwm.ChangeDutyCycle(0)
```

### `change_intensity(value)`
Called every time the slider moves. Only changes brightness if the Living Room checkbox is currently ON this prevents the slider from accidentally turning on the light when it is unchecked.

```python
def change_intensity(value):
    if living_var.get():
        living_pwm.ChangeDutyCycle(float(value))
```

### `toggle(pin, var)`
Unchanged from Task 5.1P. Used for Bathroom and Closet. Directly maps the `BooleanVar` (True/False) from the checkbox to the GPIO pin state.

```python
def toggle(pin, var):
    GPIO.output(pin, var.get())
```

### `exit_app()`
Extended from Task 5.1P. Now calls `living_pwm.stop()` before `GPIO.cleanup()` to properly shut down the PWM signal before resetting the pins.

```python
def exit_app():
    living_pwm.stop()
    GPIO.cleanup()
    window.destroy()
```

### GUI / Main Loop
The GUI is styled with a dark theme using cardstyle frames for each room. The Living Room card contains a `Checkbutton` and a `tk.Scale` slider (0-100). Bathroom and Closet cards each contain only a `Checkbutton`. The Exit button safely shuts down everything. `window.mainloop()` keeps the GUI running.

## Configuration

To use different GPIO pins, update the setup lines and the PWM object at the top of the script:

```python
GPIO.setup(18, GPIO.OUT)        # change 18 to your Living Room pin (must support PWM)
GPIO.setup(27, GPIO.OUT)        # change 27 to your Bathroom pin
GPIO.setup(22, GPIO.OUT)        # change 22 to your Closet pin

living_pwm = GPIO.PWM(18, 1000) # update 18 here too, and adjust frequency if needed
```

Also update the matching pin numbers in the `toggle()` calls for Bathroom and Closet.

> For hardware PWM, only **GPIO 12** and **GPIO 18** are supported on the Raspberry Pi. For other pins, software PWM via `GPIO.PWM()` will still work but may be slightly less smooth.

**Sihina Sithum K A**
*Deakin University - Software Engineering*
*April 2026*
