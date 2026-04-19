import RPi.GPIO as GPIO
import tkinter as tk

# Setup
GPIO.setmode(GPIO.BCM)
GPIO.setup(17, GPIO.OUT)
GPIO.setup(27, GPIO.OUT)
GPIO.setup(22, GPIO.OUT)

def turn_on(pin):
    GPIO.output(17, False)
    GPIO.output(27, False)
    GPIO.output(22, False)
    GPIO.output(pin, True)

def exit_app():
    GPIO.cleanup()
    window.destroy()

# Window
window = tk.Tk()
window.title("Light Controller")
window.geometry("280x220")
window.configure(bg="#f0f0f0")
window.resizable(False, False)
window.protocol("WM_DELETE_WINDOW", exit_app)

tk.Label(window, text="Select a room", font=("Helvetica", 14, "bold"),
         bg="#f0f0f0").pack(pady=(20, 10))

selected = tk.IntVar()

tk.Radiobutton(window, text="Living Room", variable=selected, value=17,
               font=("Helvetica", 12), bg="#f0f0f0",
               command=lambda: turn_on(17)).pack(anchor="w", padx=60)

tk.Radiobutton(window, text="Bathroom", variable=selected, value=27,
               font=("Helvetica", 12), bg="#f0f0f0",
               command=lambda: turn_on(27)).pack(anchor="w", padx=60)

tk.Radiobutton(window, text="Closet", variable=selected, value=22,
               font=("Helvetica", 12), bg="#f0f0f0",
               command=lambda: turn_on(22)).pack(anchor="w", padx=60)

tk.Button(window, text="Exit", font=("Helvetica", 11), command=exit_app,
          bg="#e74c3c", fg="white", relief="flat", padx=20).pack(pady=20)

window.mainloop()