# Lab 4, Part B: MQTT Communication with Python on Raspberry Pi

## Prerequisite:
You previously installed Mosquitto as a MQTT message broker and configured Mosquitto to allow remote connections in [Lab 4, Part A](LAB4A_MQTT_and_Mosquitto.md).

## Objective
In this session, you will create Python programs to subscribe to and publish messages to the MQTT message broker running on the Raspberry Pi. Additionally, you will connect to the Raspberry Pi command line through the terminal (or via SSH from your Windows or macOS command line).

## Materials
- Raspberry Pi 4 with Raspberry Pi OS installed and Mosquitto MQTT broker configured (as per the previous session).
- Windows or macOS computer.

## Procedure

**Creating a Python MQTT Subscriber**
1. On the Raspberry Pi, create and open a Python script for subscribing to MQTT messages. Use the `nano` text editor or any other text editor you prefer.
```bash
nano mqtt_subscriber.py
```

2. Copy and paste the following Python code into the `mqtt_subscriber.py` file. This code subscribes to an MQTT topic and displays incoming messages.
```python
import paho.mqtt.client as mqtt

# Define the MQTT broker and topic
broker_address = "localhost"  # Replace with the Raspberry Pi's IP if not running locally
topic = "conveyor1/motor1/temperature"  # Replace with the desired MQTT topic

# Callback functions for MQTT client
def on_connect(client, userdata, flags, rc):
   print("Connected to MQTT broker with result code " + str(rc))
   client.subscribe(topic)

def on_message(client, userdata, message):
   print(f"Received message on topic '{message.topic}': {message.payload.decode()}")

# Create an MQTT client
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION1)

# Set up callback functions
client.on_connect = on_connect
client.on_message = on_message

# Connect to the MQTT broker
client.connect(broker_address)

# Start the MQTT client loop to receive messages
client.loop_forever()
```

3. Save the file and exit the text editor.

4. Run the subscriber program on the Raspberry Pi:
```bash
python3 mqtt_subscriber.py
```
   
**OOPS!**

If you've never worked with the `paho-mqtt` library, the program certainly failed to start and you saw errors at the command prompt.
When attempting to run your Python program, you might encounter the following error:

```
Traceback (most recent call last):
  File "/home/youraccountname/mqtt_subscriber.py", line 1, in <module>
    import paho.mqtt.client as mqtt
ModuleNotFoundError: No module named 'paho'
```

**Error Explanation:**

This error message indicates a `ModuleNotFoundError`. Specifically, Python is unable to locate the `paho` module. The `paho` module is essential for MQTT communication in your program, and this error occurs when the module is not installed or not accessible within your Python environment. To resolve this issue, you should ensure that the `paho-mqtt` library is correctly installed in your Python environment as instructed earlier in the lab.

**Libraries**

Libraries contain pre-written code with functions designed to perform specific tasks in a programming language.
The Python community has developed numerous libraries to extend Python's capabilities for various applications.
To utilize these libraries, they must be installed as part of the configuration process for the Python runtime environment.

**Solution**

In our laboratory session, we will install the `paho-mqtt` library.
This library is essential for our tasks, enabling us to work with the MQTT protocol in Python.
Before running the provided programs, it's crucial to install the `paho-mqtt` library using `pip3`.

Before we can do this, however, we must create a virtual environment to avoid causing conflicts among the operating system.
To create the virtual environment, run the following command:
```bash
python3 -m venv myenv
```
This will create a virtual environment description called `myenv` in the `myenv` folder under the current folder.

To enter the virtual environment, run the following command
```bash
source myenv/bin/activate
```

To exit the virtual environment, you can run the `deactivate` command, but we won't be needing to do this right now.

Now that we are in the virtual environment, we may install the wanted library using `pip3`, as follows:
```bash
pip3 install paho-mqtt
```
This will ensure that the essential MQTT functionality is made accessible for your Python programs, enabling you to interact with MQTT brokers effectively.
With `paho-mqtt` successfully installed, the library is now available within the myenv environment, for this project folder.

You should now be able to run the python program with the command
```bash
python3 mqtt_subscriber.py
```

**Python MQTT Publisher:**
1. On another Raspberry Pi terminal (or via a new SSH), create and open a Python script for publishing MQTT messages. Use the `nano` text editor or your preferred text editor.
```bash
nano mqtt_publisher.py
```

2. Copy and paste the following Python code into the `mqtt_publisher.py` file. This code publishes random numbers to an MQTT topic every second.
```python
import paho.mqtt.publish as publish
import random
import time

# Define the MQTT broker and topic
broker_address = "localhost"  # Replace with the Raspberry Pi's IP if not running locally
topic = "conveyor1/motor1/temperature"  # Replace with the desired MQTT topic

while True:
   try:
       # Generate a random number between 0 and 100
       random_number = random.randint(0, 100)

       # Publish the random number to the MQTT topic
       publish.single(topic, payload=str(random_number), hostname=broker_address)

       print(f"Published: {random_number}")

       # Wait for one second before publishing the next number
       time.sleep(1)
   except KeyboardInterrupt:
       print("Publishing stopped.")
       break
```

3. Save the file and exit the text editor.

4. Run the publisher program on the Raspberry Pi:
```bash
python3 mqtt_publisher.py
```

5. You should have gotten an error if you followed the exact steps. What is the solution? Don't follow all the steps from above, because now, the virtual environment is created and paho-mqtt is installed. You only need to activate the virtual environment with the command
```bash
source myenv/bin/activate
```

6. Again, try running the publisher program on the Raspberry Pi:
```bash
python3 mqtt_publisher.py
```

Compare the two terminal windows and explain what is going on.

**Review Questions:**
Update the **reflection log** that you created in your private GitHub repository, irst answering the following questions.
1. How did you connect to the Raspberry Pi from your Windows or macOS command line?
2. Explain the purpose of the MQTT subscriber program.
3. What is the role of the MQTT publisher program?
4. How can you modify the MQTT topic and message payload in the publisher program to suit your needs?
5. What are the key components of the MQTT Python programs you created, and what is their purpose?
6. How could these MQTT communication techniques be applied in real-world IoT projects?

Ensure you document your observations and any issues encountered during this laboratory exercise in your report.

Next, think about the experience and write a few short sentences stating
- what you have learned,
- what you believe you need to improve,
- what the teacher could have said or done to make learning easier,
- what you could have done to make the learning easier, and
- other reflections that you find relevant to your personal development.

Also, list the Linux AND Python commands that you learned and write what you think they do exactly.

**Challenge**
In [Lab 3](LAB3_Controlling_LEDs_w_RPi_and_Python.md), we lit up led lights to accommodate a countup to 59.

1- Modify the publisher program to send random numbers from 0 to 7.
2- Modify the subscriber program to output the random number to the led lights as in the previous lab.

Hint: you may have to convert datatypes with the int() function to get this program working.

Update your reflection log once you are done with this challenge, thinking thinking of the same questions as above.
