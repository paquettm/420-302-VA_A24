# IoT Assignment 2: Raspberry Pi Historian Web Visualisation with ESP32 I2C Sensor

This assignment will be done at home and in class together.
You will implement this deliverable as part of your term project.

In this assignment,
- an analog signal will be sampled and sent over MQTT by an ESP32.
- a Python historian program will log all MQTT messages to a SQLite3 database on the Raspberry Pi.
- concurrently, a Python controller will send instructions over MQTT when instruction conditions are met.
- a Python program will produce a graph representation of the data in the SQLite3 database and make this view available to a Web Browser.

## Requirements

- A Raspberry Pi with Python3 installed and connected to the internet
- An ESP32 with Arduino IDE installed and connected to the computer
- A breadboard, a LED, a resistor, some jumper wires, and a potentiometer
- A configuration file named `config.json` that contains a collection of configurations consisting of conditions-results lists
- Mosquitto MQTT broker must already be installed on the Raspberry Pi and ready to accept connections from outside devices.

## Tasks

### Task 1: The IoT Controller
We will use the [IoT_Controller.py](https://github.com/paquettm/IoT_Controller_A24/blob/main/IoT_Controller.py) program from my [IoT_Controller_A24](https://github.com/paquettm/IoT_Controller_A24) repository.
This Python program does the following:
- Reads the configuration file `config.json`
- Subscribes to all MQTT topics
- Defines a callback function triggered when a new message is received
- In the callback function, checks all configurations, and if all conditions from a configuration are satisfied, publishes the MQTT messages from the results

This is already implemented, but you must complete the configuration to fulfill the requirements stated in the following tasks.

### Task 2: The Sensor

- Write an Arduino sketch named `sensor.ino` that does the following:
    - Connects to the WiFi network using the `WiFi.h` library
    - Connects to the MQTT broker on the Raspberry Pi using the `PubSubClient.h` library
    - Reads an analog value, for example,
        - the voltage from a potentiometer or photocell connected as a voltage divider to the pin 34 on the ESP32 or
        - the value sampled by an I2C device (SDA and SCL normally on GPIO pins 21 and 22, respectively).
    - Publishes the value to the topic name that identifies the variable you wish to measure in your project, at an interval relevant to the control of the targeted process. For example, if your projected system must measure aquarium water temperature, publishing to "aquarium/temperature" once per minute could be reasonable.

### Task 3: The Actor

**Note: You may implement the actor logic in the Raspberry Pi or in an ESP32.**

Write a Python program named `actor.py` or an arduino program that does the following:
- Subscribes to the topic name appropriate to your project output, using the `paho-mqtt` library in Python or the PubSub library in Arduino C. For example, if you will control an aquarium heater in your project, the topic could be "aquarium/heater"
- Defines a callback function that is triggered when a new message is received.
- In the callback function, sets the GPIO pin 18 to output mode and writes a `1` signal to it if the message payload is `on`, or a `0` signal if the message payload is `off`
- Connects the LED and the resistor to the GPIO pin 18 and the ground pin on the Raspberry Pi using the breadboard and the jumper wires

### Task 4: The Historian

You will use the [historian.py](https://github.com/paquettm/IoT_Controller_A24/blob/main/historian.py) program from my [IoT_Controller_A24](https://github.com/paquettm/IoT_Controller_A24) repository.
This program will record all messages to the historian_data.db SQLite3 database.

### Task 5: The Web Visualisation

You will follow the procedure from [Lab 8](../LABS/LAB8_Web_Historian_Visualisation.md) to implement and run the Visualisation Web interface.

## Testing

- Run the `IoT_Controller.py` program on the Raspberry Pi
- Upload and run the `sensor.ino` sketch on the ESP32
- Run the `actor.py` program on the Raspberry Pi (or include the same logic to the ESP32 sketch if this was your choice)
- Observe the LED and the messages on the MQTT broker
- Change the configuration file `config.json` to use different conditions and results
- Test if the LED behaves as expected according to the configuration file
- Observe the Web interface

## Submission

Create a new GitHub repository and include all content from this assignment.
Invite the teacher as a contributor.
Complete a reflection log for this activity.

Deadline November 30, 2023, 11:55PM.
