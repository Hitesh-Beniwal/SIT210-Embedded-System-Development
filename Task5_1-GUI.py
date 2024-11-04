import tkinter as tk
import RPi.GPIO as GPIO

# Initialize GPIO mode and configuration
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

# Define GPIO pins for each LED color
LED_PINS = {'Red': 0, 'Green': 5, 'Blue': 6}

# Set up each LED pin as output
for pin in LED_PINS.values():
    GPIO.setup(pin, GPIO.OUT)

# Turn off all LEDs by setting pins to LOW
def turn_off_leds():
    for pin in LED_PINS.values():
        GPIO.output(pin, GPIO.LOW)

# Update LEDs based on selected color
def update_leds(selected_color):
    turn_off_leds()
    if selected_color in LED_PINS:
        GPIO.output(LED_PINS[selected_color], GPIO.HIGH)

# Initialize GUI window
window = tk.Tk()
window.title("LED Control Panel")

# Variable to hold the selected color
color_choice = tk.StringVar(value="None")

# Create radio buttons for each LED color
for color in LED_PINS.keys():
    tk.Radiobutton(
        window, text=f"{color} LED", variable=color_choice,
        value=color, command=lambda: update_leds(color_choice.get())
    ).pack(anchor=tk.W)

# Exit button to close the GUI and cleanup GPIO
def close_program():
    turn_off_leds()
    GPIO.cleanup()
    window.destroy()

tk.Button(window, text="Exit", command=close_program).pack()

# Start the GUI event loop
window.mainloop()
