# =============================================
# Linda's Room — Carer Dashboard
# Raspberry Pi
# =============================================

import tkinter as tk
import paho.mqtt.client as mqtt
import threading
import time

# --- MQTT settings ---
BROKER       = "broker.emqx.io"
PORT         = 1883
TOPIC_TEMP   = "linda/fan/temperature"
TOPIC_MOTION = "linda/fan/motion"
TOPIC_SWITCH = "linda/fan/switch_state"
TOPIC_SPEED  = "linda/fan/speed_pct"
TOPIC_TIME   = "linda/fan/time_mode"
TOPIC_MODE   = "linda/fan/cmd/mode"
TOPIC_MANUAL = "linda/fan/cmd/manual_speed"

DANGER_TEMP      = 32.0
current_mode     = "AUTO"
last_slider_val  = -1
last_slider_sent = 0

# --- Fan speed colour ---
def speed_color(speed):
    try:
        s = int(speed)
        if s < 40:   return "#44bb44"
        elif s < 70: return "#ffaa00"
        else:        return "#ff4444"
    except:
        return "#ffffff"

# --- Alert check ---
def check_alert(temp):
    try:
        if float(temp) >= DANGER_TEMP:
            alert_label.config(text=f"⚠  DANGER: Room is {temp}°C!", bg="#cc0000", fg="white")
        else:
            alert_label.config(text="✓  All clear", bg="#1a3a1a", fg="#44bb44")
    except:
        pass

# --- Connection status ---
def set_status(connected):
    if connected:
        conn_label.config(text="● Connected", fg="#44bb44")
    else:
        conn_label.config(text="● Disconnected", fg="#ff4444")

# --- MQTT callbacks ---
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        client.subscribe(TOPIC_TEMP)
        client.subscribe(TOPIC_MOTION)
        client.subscribe(TOPIC_SWITCH)
        client.subscribe(TOPIC_SPEED)
        client.subscribe(TOPIC_TIME)
        root.after(0, lambda: set_status(True))
    else:
        root.after(0, lambda: set_status(False))

def on_disconnect(client, userdata, rc):
    root.after(0, lambda: set_status(False))

def on_message(client, userdata, msg):
    topic   = msg.topic
    payload = msg.payload.decode()

    # Update each label immediately on message arrival
    if topic == TOPIC_TEMP:
        root.after(0, lambda: temp_val.config(text=f"{payload} °C"))
        root.after(0, lambda: check_alert(payload))

    elif topic == TOPIC_MOTION:
        text  = "OCCUPIED" if payload == "1" else "EMPTY"
        color = "#44bb44"  if payload == "1" else "#ff4444"
        root.after(0, lambda: occupancy_val.config(text=text, fg=color))

    elif topic == TOPIC_SWITCH:
        color = "#44bb44" if payload == "ON" else "#ff4444"
        root.after(0, lambda: switch_val.config(text=payload, fg=color))

    elif topic == TOPIC_SPEED:
        root.after(0, lambda: speed_val.config(
            text=f"{payload} %", fg=speed_color(payload)))

    elif topic == TOPIC_TIME:
        root.after(0, lambda: timemode_val.config(text=payload))

# --- Send mode command ---
def set_mode(mode):
    global current_mode
    current_mode = mode
    mqtt_client.publish(TOPIC_MODE, mode)
    update_buttons()
    update_slider()

# --- Button highlight ---
def update_buttons():
    colors = {"AUTO": "#1a3a1a", "MANUAL": "#1a1a3a", "OFF": "#3a1a1a"}
    text_colors = {"AUTO": "#44bb44", "MANUAL": "#4444ff", "OFF": "#ff4444"}
    for m, btn in mode_buttons.items():
        if m == current_mode:
            btn.config(bg=colors[m], fg=text_colors[m], relief="sunken", bd=2)
        else:
            btn.config(bg="#3a3a3a", fg="#888888", relief="raised", bd=1)

# --- Slider active only in MANUAL ---
def update_slider():
    if current_mode == "MANUAL":
        slider.config(state="normal", fg="#ffffff", troughcolor="#334")
    else:
        slider.config(state="disabled", fg="#555555", troughcolor="#333")

# --- Slider — send immediately, throttle to max once per 150ms ---
def on_slider(val):
    global last_slider_val, last_slider_sent
    if current_mode != "MANUAL":
        return
    int_val = int(float(val))
    now     = time.time() * 1000
    if int_val != last_slider_val and (now - last_slider_sent) >= 150:
        last_slider_val  = int_val
        last_slider_sent = now
        mqtt_client.publish(TOPIC_MANUAL, str(int_val))

# --- Build GUI ---
root = tk.Tk()
root.title("Linda's Room — Carer Dashboard")
root.configure(bg="#1a1a1a")
root.geometry("500x580")
root.minsize(420, 520)

BG       = "#1a1a1a"
PANEL_BG = "#252525"
FG       = "#ffffff"
LABEL_FG = "#888888"
FONT     = ("Helvetica", 13)
FONT_B   = ("Helvetica", 13, "bold")
FONT_H   = ("Helvetica", 19, "bold")

root.columnconfigure(0, weight=1)
root.rowconfigure(1, weight=1)
root.rowconfigure(2, weight=1)

# --- Title row ---
title_frame = tk.Frame(root, bg=BG)
title_frame.grid(row=0, column=0, sticky="ew", padx=20, pady=(16, 4))
title_frame.columnconfigure(0, weight=1)

tk.Label(title_frame, text="LINDA'S ROOM", font=FONT_H,
         bg=BG, fg=FG).grid(row=0, column=0, sticky="w")

conn_label = tk.Label(title_frame, text="● Connecting...",
                       font=("Helvetica", 11), bg=BG, fg="#ffaa00")
conn_label.grid(row=0, column=1, sticky="e")

# --- Status panel ---
status_frame = tk.Frame(root, bg=PANEL_BG, padx=20, pady=16)
status_frame.grid(row=1, column=0, sticky="nsew", padx=20, pady=(4, 6))
status_frame.columnconfigure(1, weight=1)

def status_row(label, row):
    tk.Label(status_frame, text=label, font=FONT, bg=PANEL_BG,
             fg=LABEL_FG, anchor="w").grid(row=row, column=0, sticky="w", pady=4, padx=(0,20))
    val = tk.Label(status_frame, text="--", font=FONT_B, bg=PANEL_BG,
                   fg=FG, anchor="w")
    val.grid(row=row, column=1, sticky="ew", pady=4)
    return val

temp_val      = status_row("Temperature",  0)
speed_val     = status_row("Fan Speed",    1)
switch_val    = status_row("Switch",       2)
occupancy_val = status_row("Occupancy",    3)
timemode_val  = status_row("Time Mode",    4)

# --- Carer controls ---
ctrl_frame = tk.Frame(root, bg=PANEL_BG, padx=20, pady=16)
ctrl_frame.grid(row=2, column=0, sticky="nsew", padx=20, pady=(0, 6))
ctrl_frame.columnconfigure(0, weight=1)

tk.Label(ctrl_frame, text="CARER CONTROLS", font=("Helvetica", 11, "bold"),
         bg=PANEL_BG, fg=LABEL_FG).grid(row=0, column=0, sticky="w", pady=(0, 10))

btn_frame = tk.Frame(ctrl_frame, bg=PANEL_BG)
btn_frame.grid(row=1, column=0, sticky="ew")
btn_frame.columnconfigure((0,1,2), weight=1)

mode_buttons = {}
for i, mode in enumerate(["AUTO", "MANUAL", "OFF"]):
    btn = tk.Button(btn_frame, text=mode, font=FONT_B,
                    bg="#3a3a3a", fg="#888888", activebackground="#444",
                    command=lambda m=mode: set_mode(m),
                    cursor="hand2", width=8, pady=6)
    btn.grid(row=0, column=i, padx=4, sticky="ew")
    mode_buttons[mode] = btn

tk.Label(ctrl_frame, text="Manual Speed", font=FONT,
         bg=PANEL_BG, fg=LABEL_FG).grid(row=2, column=0, sticky="w", pady=(16, 2))

slider = tk.Scale(ctrl_frame, from_=0, to=100, orient="horizontal",
                  bg=PANEL_BG, fg="#555555", troughcolor="#333333",
                  highlightthickness=0, command=on_slider,
                  state="disabled", font=FONT)
slider.grid(row=3, column=0, sticky="ew", pady=(0, 4))

# --- Alert bar ---
alert_label = tk.Label(root, text="✓  All clear", font=FONT_B,
                        bg="#1a3a1a", fg="#44bb44", pady=10)
alert_label.grid(row=3, column=0, sticky="ew", padx=20, pady=(0, 16))

# --- MQTT on background thread ---
mqtt_client = mqtt.Client(client_id=f"linda_dashboard_{int(time.time())}")
mqtt_client.on_connect    = on_connect
mqtt_client.on_disconnect = on_disconnect
mqtt_client.on_message    = on_message
mqtt_client.connect_async(BROKER, PORT, keepalive=30)
mqtt_thread = threading.Thread(target=mqtt_client.loop_forever, daemon=True)
mqtt_thread.start()

update_buttons()
update_slider()

root.mainloop()
