import RPi.GPIO as GPIO
import tkinter as tk

#  Setup 
GPIO.setmode(GPIO.BCM)
GPIO.setup(18, GPIO.OUT)
GPIO.setup(27, GPIO.OUT)
GPIO.setup(22, GPIO.OUT)

living_pwm = GPIO.PWM(18, 1000)
living_pwm.start(0)

#  Colours 
BG      = "#1e1e2e"   
CARD    = "#2a2a3d"   
ACCENT  = "#f5a623"   
TEXT    = "#ffffff"
SUBTEXT = "#a0a0b0"
BTN_RED = "#e74c3c"


#  Functions 
def toggle_living():
    if living_var.get():
        living_pwm.ChangeDutyCycle(intensity_slider.get())
    else:
        living_pwm.ChangeDutyCycle(0)

def change_intensity(value):
    if living_var.get():
        living_pwm.ChangeDutyCycle(float(value))

def toggle(pin, var):
    GPIO.output(pin, var.get())

def exit_app():
    living_pwm.stop()
    GPIO.cleanup()
    window.destroy()


#  Window 
window = tk.Tk()
window.title("Light Controller")
window.geometry("340x400")
window.configure(bg=BG)
window.resizable(False, False)
window.protocol("WM_DELETE_WINDOW", exit_app)

#  Title 
tk.Label(window, text="💡  Light Controller",
         font=("Helvetica", 16, "bold"),
         bg=BG, fg=ACCENT).pack(pady=(24, 4))

tk.Label(window, text="Select rooms to control",
         font=("Helvetica", 10),
         bg=BG, fg=SUBTEXT).pack(pady=(0, 16))


#  card frame for rooms
def make_card(parent):
    frame = tk.Frame(parent, bg=CARD, padx=16, pady=12)
    frame.pack(fill="x", padx=24, pady=6)
    return frame


#  Living Room Card  
living_card = make_card(window)
living_var = tk.BooleanVar()

tk.Checkbutton(living_card, text="  Living Room",
               variable=living_var,
               font=("Helvetica", 12, "bold"),
               bg=CARD, fg=TEXT,
               selectcolor=CARD,
               activebackground=CARD,
               activeforeground=ACCENT,
               command=toggle_living).pack(anchor="w")

intensity_slider = tk.Scale(
    living_card,
    from_=0, to=100,
    orient="horizontal",
    label="Intensity %",
    font=("Helvetica", 9),
    bg=CARD, fg=SUBTEXT,
    troughcolor="#3d3d55",
    highlightthickness=0,
    activebackground=ACCENT,
    length=260,
    command=change_intensity
)
intensity_slider.set(50)
intensity_slider.pack(pady=(6, 0))


#  Bathroom Card 
bathroom_card = make_card(window)
bathroom_var = tk.BooleanVar()

tk.Checkbutton(bathroom_card, text="  Bathroom",
               variable=bathroom_var,
               font=("Helvetica", 12, "bold"),
               bg=CARD, fg=TEXT,
               selectcolor=CARD,
               activebackground=CARD,
               activeforeground=ACCENT,
               command=lambda: toggle(27, bathroom_var)).pack(anchor="w")


#  Closet Card 
closet_card = make_card(window)
closet_var = tk.BooleanVar()

tk.Checkbutton(closet_card, text="  Closet",
               variable=closet_var,
               font=("Helvetica", 12, "bold"),
               bg=CARD, fg=TEXT,
               selectcolor=CARD,
               activebackground=CARD,
               activeforeground=ACCENT,
               command=lambda: toggle(22, closet_var)).pack(anchor="w")


#  Exit Button 
tk.Button(window, text="Exit", font=("Helvetica", 11, "bold"),
          command=exit_app,
          bg=BTN_RED, fg=TEXT,
          relief="flat", padx=30, pady=6,
          activebackground="#c0392b",
          activeforeground=TEXT,
          cursor="hand2").pack(pady=20)

window.mainloop()
