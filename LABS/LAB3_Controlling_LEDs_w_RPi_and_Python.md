# Lab: Controlling LEDs with Raspberry Pi and Python

Before this lab, you should have gotten an overview of programming in Python.
Now, you will be trying to understand a bit of an existing program and modify it slightly to accomplish a related task.

This lab will teach students how to control LEDs using Python GPIO on a Raspberry Pi and display a binary count of seconds from 0 to 59 and then from 0 to 8.

**Objective:** In this lab, you will learn how to control LEDs using Python on a Raspberry Pi. You will write a Python program to display a binary count of seconds from 0 to 8 using LEDs.

**Materials Needed:**
- Raspberry Pi (any model with GPIO pins)
- Breadboard
- 6 LEDs
- 6 current-limiting resistors (330 ohms to 1k ohm) 
- Jumper wires

**Instructions:**

**1. Hardware Setup:**
a. Connect the Raspberry Pi to a power source and ensure it's properly configured.
b. Using a breadboard and/or Dupont leads, assemble the circuit as follows:
Refer to the pinout at [https://www.raspberrypi.com/documentation/computers/raspberry-pi.html](https://www.raspberrypi.com/documentation/computers/raspberry-pi.html#gpio) for GPIO logical pin names. The section is called **GPIO and the 40-pin header**.
- Connect the anode (longer lead) of each LED to individual GPIO pins on the Raspberry Pi (use the pins with BCM (logical) numbering 2, 3, 4, 14, 15, and 18).
- Connect the cathode (shorter lead) of each LED to a current-limiting resistor.
- Connect the other end of each resistor to the ground (GND) pin on the Raspberry Pi.

Ensure that the LEDs are connected in the correct order as per the BCM (logical) pin numbers.

**2. Python Code:**
a. Open the terminal on your Raspberry Pi. Search for this terminal in the top-left raspberry-icon menu.
b. Create a new Python file for your program by editing a new file with the `nano` text editor:

```bash
nano binary_seconds_display.py
```

c. Here is an example that takes the 0-59 seconds count and displays it on 6 LEDs through a 6-pin output. Copy and paste the following Python code into the `binary_seconds_display.py` file:

```python
import RPi.GPIO as GPIO # General Purpose Input/Output library
import time

# Set up the GPIO mode
GPIO.setmode(GPIO.BCM)

# Define the GPIO pins for each LED
led_pins = [2, 3, 4, 14, 15, 18]

# Initialize GPIO pins as outputs
for pin in led_pins:
    GPIO.setup(pin, GPIO.OUT)

try:
    while True: # forever until CTRL-C
        # get the current time and seconds
        current_time = time.localtime()
        seconds = current_time.tm_sec

        # convert seconds to binary string
        binary_seconds = format(seconds, '06b')

        # display each binary digit by turning LEDs on or off
        for i in range(6):
            GPIO.output(led_pins[i], int(binary_seconds[i]))

        # wait one second before resuming the program
        time.sleep(1)

finally: # when an error occurs, don't leave the try block without doing this
    # Clean up GPIO on program exit
    GPIO.cleanup()
```

Save the file by pressing the `CTRL-o` key combination (holding `CTRL` down while pressing and releasing `o`).
Exit the `nano` text editor by pressing the `CTRL-x` key combination (holding `CTRL` down while pressing and releasing `x`).

**3. Running the Program:**
a. Back in the terminal, run the Python program file that you just created with the following command:

```bash
python3 binary_seconds_display.py
```

Instead of typing the entire command, type only 
```bash
python3 bin
```
and then press the `TAB` key to attempt autocomplete. It may work and complete the name of the file that is located in the present working directory, i.e., in the folder where you are currently working in the terminal.

Your LEDs should now start displaying part of a binary count of seconds from 0 to 59. Each LED represents a binary digit, and they will change to display the current seconds.

**4. Modify the Program**

We want to change this program, to understand it well. We must get a count from 0 to 8.

How many LEDs do we need to display a count from 0 to 8?
What about from 0 to 10?

You must now modify this program slightly, possibly:
- adding a mathematical operation,
- modifying the conversion code, and/or
- changing some looping parameters
to get output only on the 3 first LEDs.
You will be counting the seconds from 0 to 8.

**5. Lab Questions:**
Observe the behavior of the LEDs.
- What do you notice as the seconds change from 0 to 59? And 59 to 0?
- What do you notice as the seconds change count from 0 to 8? Is the behaviour always the same? Why?
- How does the `format(seconds, '06b')` function work in formatting the binary representation?
- What would you need to modify in the code if you wanted to use different GPIO pins for the LEDs?
