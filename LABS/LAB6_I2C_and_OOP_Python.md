# Lab 6: Communicating with I2C Devices on Raspberry Pi in Object-Oriented Python

This lab assumes previous knowledge of [I2C](https://en.wikipedia.org/wiki/I%C2%B2C).

For Raspberry Pi GPIO pinout, refer to the [following](https://cdn.sparkfun.com/assets/learn_tutorials/1/5/9/5/GPIO.png).


## Objective:
In this lab, you will learn how to
- enable the I2C interface on your Raspberry Pi,
- connect an I2C temperature sensor, and 
- write a Python program to communicate with the sensor and read temperature data.

Moreover, you will see an example of object-oriented programming (OOP) in Python.

### Prerequisites:
- Raspberry Pi running Raspberry Pi OS (Raspbian)
- An I2C temperature sensor (e.g., MCP9808)
- Jumper wires

Note: You may use any other I2C-connected measuring device for this lab, but you will have to adapt the algorithms to match the measurements and conversions required.

### Step 1: Enable the I2C Interface

On your Raspberry Pi

1.1. Open a terminal, for example by pressing the `CTRL-ALT-t` key combination in the Raspberry Pi OS GUI.

1.2. Run the following command to launch the Raspberry Pi Configuration tool:
   
   ```bash
   sudo raspi-config
   ```

1.3. With the arrow keys, select "Interface Options" and press `enter`.

1.4. With the arrow keys, select "I2C" and press `enter`.

1.5. When prompted, use the arrow keys to select "Yes" and press `enter`. This will enable the I2C interface on your Raspberry Pi. You should see the message that `The ARM I2C interface is enabled`. Press `enter`.

1.6. With the arrow keys, select "Finish" and press `enter`.

1.7. Reboot your Raspberry Pi to apply the changes with the following command:

   ```bash
   sudo reboot
   ```

### Step 2: Connect the I2C Device

2.1. **Physical Connections**:
   - Connect the SDA (Serial Data) pin of the I2C sensor to GPIO pin 2 (physical pin 3) on your Raspberry Pi.
   - Connect the SCL (Serial Clock) pin of the I2C sensor to GPIO pin 3 (physical pin 5) on your Raspberry Pi.
   - Connect the sensor to VCC and GND (optionally from the RPi physical pins 1 for 3V3, 2 or 3 for 5V and 6, 9, 14, 20, 25, 30, 34, or 39 for Ground)
   - Ensure that your Raspberry Pi and I2C sensor share a common ground connection.
   - Ground all address pins from the sensor (A0, A1, A2). These are used to set addresses for use of multiple units.

Pins 2 and 3 (resp physical pins 3 and 5) are used as examples in this lab. If these are nto available in your setup for any reason, you may use other pins that are available for this purpose and adapt the Python3 code below to the used logical (BCM) pin numbers.

2.2 **Test I2C Connection**
Open a terminal on your Raspberry Pi.

Test the I2C connection to verify that your Raspberry Pi can detect the I2C sensor using the i2cdetect command. Run the following command:

```bash
sudo i2cdetect -y 1
```

This command will display a grid showing detected I2C device addresses.
Ensure that your I2C sensor's address appears in the grid, indicating a successful connection.
If it doesn't, double-check your connections and address settings.

For reference, for the device used in this lab, you should see the number `18` appear at the intersection of line 10 and column 8.

Notice that the columns are numbered `0` to `9` and `a` to `f`.
This is because the address numbers are [**hexadecimal**](https://en.wikipedia.org/wiki/Hexadecimal).

### Step 3: Install the SMBus Library

On your Raspberry Pi

   3.1. Open a terminal, for example by pressing `CTRL-ALT-t`.

   3.2. Create a new folder for this lab and enter it.
   ```
   mkdir lab6
   ```
   will create the directory and
   ```
   cd lab6
   ```
   will enter it.

   3.3. Virtual environment creation and activation

   We must create a virtual environment to contain libraries, avoiding causing conflicts with other programs with conflicting definitions within the same operating system.
   To create the virtual environment, run the following command:
   ```bash
   python3 -m venv --system-site-packages i2c
   ```
   This will create a virtual environment description called `i2c` in the `i2c` folder under the current folder.
   We include the `--system-site-packages` option so that all system-wide packages are also accessible within this virtual environment.
   In other words, if a library already exists in the Raspberry Pi OS, it can be accessed in this environment.
   This will be handy for a specific error we are likely to face in a minute.

   You can choose another name than `i2c`.

   To enter the virtual environment, run the following command
   ```bash
   source i2c/bin/activate
   ```
   ...but if you chose a name that is different from `i2c`, make sure to substitute `i2c` for that name in the instruction above.

   3.4. With the the virtual environment activated, install the `smbus` library using `pip3`. This library can be used, in Python, to communicate with I2C devices:

   ```bash
   sudo pip3 install smbus
   ```
   
   BUT... If you get an error message stating that the library is externally managed, this means that the library was already installed system-wide.
   That is fine because we have included the `--system-site-packages` option in our virvual environment creation instruction.

### Step 4: Write Python Code to Read Temperature
+
4.1. Create a Python script (e.g., `temperature_sensor.py`) using your preferred text editor. You can use the `nano` text editor:

   ```bash
   nano temperature_sensor.py
   ```

4.2. The following Python3 program looks a bit different from the programs we worked with previously: it is object-oriented.

What does object-oriented mean? Before we continue, take 5 minutes to [watch this video](https://www.youtube.com/watch?v=X3cFiJnxUBY).

In our own code, as below, we define `class TemperatureSensor`, which is a blueprint for the `TemperatureSensor` type of `object`.

In class definitions, we write what the instances, or objects of that class will have as `properties` and `methods`.
Properties are the variables that each object will have to store data. Methods are the actions that objects can apply, if called.

In particular, our `TemperatureSensor` class defines the following functions:
- _read_sensor_data(self): a function to acquire data from the I2C temperature sensor.
- get_temperature(self): a function that calls the above and converts the reading to degrees celcius.
- __init__(self, ...): a special function called the `constructor`. This function is called each time a new `object` of the `class TemperatureSensor` is created.

For each of these functions, the first argument is `self`.
This argument serves to receive the reference to the object, forwarded by Python3 when the object function is called.
Since the functions are called on the objects, we call them `methods` instead of `functions`.

You may notice, in the constructor, that variables `self.i2c_bus` and `self.i2c_address` are initialized to the values passed to the constructor through the arguments `i2c_bus, i2c_address` listed after `self`.
These variables (`self.i2c_bus` and `self.i2c_address`) are properties. 
We recognize properties because they are variables that are called in reference to `self`; they are variables that are attached to the object.

A program that includes a class definition will not accomplish anything unless the class is instantiated into objects and possibly other methods are called.

Notice that, after the class is defined, the indent reduces to the same level than it was on the line where `class TemperatureSensor` was declared.
The three first instructions after the class declaration define 2 variables and a new class instance, `sensor`.
Then, the `get_temperature` method of `sensor` is called in an infinite loop, to obtain the temperature.
This value is formatted and output to the screen in the `print(...)` statement.

In the context described above, try to understand and transcribe the following Python3 code into a Python (.py) script file:

   ```python
import smbus
import time

class TemperatureSensor:
    def __init__(self, i2c_bus, i2c_address):
        # Constructor to initialize the sensor with I2C bus and address
        self.i2c_bus = i2c_bus
        self.i2c_address = i2c_address

    def get_temperature(self):
        # Method to convert temperature data from the I2C sensor
        data = self._read_sensor_data()
        temperature = ((data[0] & 0x0F) << 8) | data[1]
        temperature /= 16.0
        return temperature

    def _read_sensor_data(self):
        # Private method to read sensor data from the I2C device
        try:
            bus = smbus.SMBus(self.i2c_bus)
            # Read temperature data from the specified I2C address
            # In the datasheet for MCP9808, the temperature is stored in the
            # 0x5 register
            data = bus.read_i2c_block_data(self.i2c_address, 0x5, 2)
            return data
        except Exception as e:
            print(f"Error reading data from I2C sensor: {e}")
            return None

# I2C bus 1 is on SDA SCL pins 3,5 (physical)
i2c_bus_number = 1
# MCP9808 has a default I2C address of 0x18 (from the datasheet)
i2c_device_address = 0x18
# Creating a TemperatureSensor instance with I2C configuration
sensor = TemperatureSensor(i2c_bus=i2c_bus_number, i2c_address=i2c_device_address)
# Invoking the get_temperature method and displaying the result

while True:
    print(f"Temperature: {sensor.get_temperature():.2f}°C")
    time.sleep(1)
   ```

4.3. Save the script and exit the text editor.


## Introduction to Object-Oriented Programming (OOP)

We explained the code above in context, we will repeat the same thing again, a bit differently, it may help you understand better.

### Object-Oriented Programming (OOP):

OOP is a programming paradigm that models real-world entities as objects with attributes and behaviors.
Objects are instances of classes, which define their attributes (data) and methods (functions).

### Explaining the Python Code:

The provided Python code demonstrates OOP principles:

TemperatureSensor is a class representing the I2C temperature sensor.
The following instruction
```
sensor = TemperatureSensor(i2c_bus=i2c_bus_number, i2c_address=i2c_device_address)
```
creates an object, called `sensor`, of the class `TemperatureSensor`.
When the object is created, we pass in configurations allowing us to interface with an actual temperature sensor.
If there were multiple temperature sensors with which to interface in our application, when we could instantiate many objects, each with their own parameters.
For example, we could have a sensor at address 0x18, another at 0x19, etc.

In class `TemperatureSensor`

- The __init__ method initializes the object. It gets called at the when we declare new objects as we did in the instruction 
```
sensor = TemperatureSensor(i2c_bus=i2c_bus_number, i2c_address=i2c_device_address)
```
This type of function is called a `constructor`.
- The get_temperature method retrieves temperature data, converts it to a usable reading in degrees celcius and returns it.
- The _read_sensor_data method reads data from the I2C sensor.

Attributes:
- i2c_bus and i2c_address are attributes storing I2C configuration.
- in the class definition, they are preceded by the keyword `self` to make it clear that they are object attributes and not global variables.


### Step 5: Run the Python Program

5.1. Run the Python script to read temperature data from the I2C sensor:

   ```bash
   python3 temperature_sensor.py
   ```

   The script will continuously display temperature readings in degrees Celsius.

### Step 6: Observations

6.1. Observe the temperature readings displayed in the terminal. The program should read and display the temperature from your I2C temperature sensor.

6.2. Experiment with different conditions to see how the temperature readings change.

### Conclusion:
In this lab, you learned how to enable the I2C interface on your Raspberry Pi, connect an I2C temperature sensor, and use Python to communicate with the sensor and read temperature data. This skill is essential for working with various I2C-based sensors and devices in your Raspberry Pi projects.

Moreover, you were able to observe code structured usng the Object-Oriented Programming approach.
This programming paradigm enables programmers to organize code in a way that is scalable and maintainable.

### Challenge

With what you have learned so far, combine code from the current lab with code from previous labs to build 2 Python programs:

1. A Python program that reads the temperature from the i2c bus and publishes it to MQTT topic "room/temperature".
2. A Python program that subscribes to the "room/temperature" topic and outputs the data to the console, non-stop.

### Challenge 2

Using 2 objects, get readings from 2 sensors on the i2c bus and proceed as above, on topics "room1/temperature" and "room2/temperature".

## Reflection log
Update your reflection log to include everything that you observed and learned in this lab.
Pay special attention to the ideas related to Object-Oriented Programming.