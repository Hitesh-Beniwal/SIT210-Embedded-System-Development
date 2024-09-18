import RPi.GPIO as GPIO  # Import GPIO library for controlling Raspberry Pi pins
import tkinter as tk     # Import tkinter library for creating the GUI

# Use Broadcom (BCM) numbering for pin assignments
GPIO.setmode(GPIO.BCM)

# Define the GPIO pins for each LED by color
LED_PINS = {'red': 17, 'green': 27, 'yellow': 22}

# Initialize all LEDs as turned off
for pin in LED_PINS.values():
    GPIO.setup(pin, GPIO.OUT)  # Set pin as output
    GPIO.output(pin, GPIO.LOW) # Start with LEDs off

# Function to control which LED is active
def switch_led(color):
    # Turn off all LEDs before activating the chosen one
    for pin in LED_PINS.values():
        GPIO.output(pin, GPIO.LOW)
    # Activate the selected LED
    GPIO.output(LED_PINS[color], GPIO.HIGH)

# Function to properly close the program
def close_program():
    GPIO.cleanup()  # Reset GPIO settings to prevent conflicts
    window.destroy()  # Close the Tkinter window

# Create a Tkinter window
window = tk.Tk()
window.title("LED Selector")

# Store the current active LED color
selected_led = tk.StringVar(value='red')

# Create buttons to select which LED to turn on
tk.Radiobutton(window, text="Red LED", variable=selected_led, value='red', command=lambda: switch_led('red')).pack(anchor=tk.W)
tk.Radiobutton(window, text="Green LED", variable=selected_led, value='green', command=lambda: switch_led('green')).pack(anchor=tk.W)
tk.Radiobutton(window, text="Yellow LED", variable=selected_led, value='yellow', command=lambda: switch_led('yellow')).pack(anchor=tk.W)

# Add an exit button to close the program
tk.Button(window, text="Exit", command=close_program).pack()

# Start the Tkinter main event loop
window.mainloop()
