# Lab 7: SQLITE3 and the IoT System Historian

## SQLITE3

A database system is a means bi which we store data easily on computer systems.
It allows the creation and use of data entities inside which it is faster and more convenient to stare data than files.
Most database systems require a database server program running in the background. Not SQLite3.

SQLite3 is a lightweight, file-based database system that doesn't require a separate server process, making it ideal for many Raspberry Pi projects.

## Installation of the SQLite3 Client

We will not be installing a server on our Raspberry Pi. We will only need a program that can interact with the database files, for our supervision purposes. Programs will not need this client installed to run correctly.

To install the SQLite3 client on Raspberry Pi OS, follow these simple steps:

1. Open the terminal on your Raspberry Pi.

2. Update your package lists by running:

```bash
sudo apt update
```

3. Install the SQLite3 package by executing:

```bash
sudo apt install sqlite3
```

This command will install the SQLite3 command-line interface and the necessary libraries.

4. After the installation is complete, you can verify it by checking the SQLite3 version:

```bash
sqlite3 --version
```

This should display the version of SQLite3 installed on your system.

## Using SQLite3

Once installed, you can start using SQLite3 from the command line. Here are a few basic commands to get you started:

- To create a new database or open an existing one:
  ```bash
  sqlite3 your_database.db
  ```

- Within the SQLite3 shell, you can use SQL commands to interact with your database. For example:
  ```sql
  .tables
  CREATE TABLE mytable (id INTEGER PRIMARY KEY, name TEXT);
  INSERT INTO mytable (name) VALUES ('Test');
  SELECT * FROM mytable;
  ```

- To exit the SQLite3 shell, use:
  ```
  .quit
  ```

## Implementing the Historian

Your teacher will show you the following program:
```python
import paho.mqtt.client as mqtt
import sqlite3
import json
from datetime import datetime

# configuration of the MQTT system
MQTT_BROKER = "localhost" #the address of the broker
MQTT_CLIENT_ID = "historian-client"
MQTT_TOPIC = "#"

# SQLite Database configuration
DB_FILE = "historian_data.db"

# MQTT client callback for connection - the method that will be run once connected to the broker
def on_connect(client, userdata, flags, rc):
    print("connected to MQTT")
    # subscribe to the topics
    client.subscribe(MQTT_TOPIC)

# MQTT client callback to handle incoming messages
def on_message(client, userdata, msg):
    print("got a message")
    #get the value
    payload = msg.payload.decode()
    #get the topic
    topic = msg.topic
    #get the timestamp
    timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    #save all that to the database as a record
    save_to_database(topic, payload, timestamp)

# method to save to the SQLite3 database
def save_to_database(topic, value, timestamp):
    print("saved a message")
    # connect to the database
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    # make sure the DB table exists and build it otherwise
    SQL = "CREATE TABLE IF NOT EXISTS historian_data (topic TEXT, payload TEXT, timestamp TEXT)"
    cursor.execute(SQL)

    # save the message to the table
    SQL = "INSERT INTO historian_data (topic, payload, timestamp) VALUES (?,?,?)"
    cursor.execute(SQL, (topic, value, timestamp))

    # confirm the writing and close
    conn.commit()
    conn.close()

#setting up the object
client = mqtt.Client(client_id=MQTT_CLIENT_ID)

#setting up the callback methods
client.on_connect = on_connect
client.on_message = on_message

#setting up the connection to the broker
client.connect(MQTT_BROKER, 1883, 60)

# start the MQTT client loop but let us define further logic
client.loop_start()

try:
    while True:
        #more logic goes here
        pass

except KeyboardInterrupt:
    #disconnect the client from the broker
    client.disconnect()
```

## Verifying the Functionality

Run the historian program:
```bash
python3 historian.py
```

### Sending messages

In another terminal window, send a couple messages over MQTT:
```bash
mosquitto_pub -t "room/temperature" -m 25
mosquitto_pub -t "room/humidity" -m 95
mosquitto_pub -t "room/lighting" -m 5
```

The program should react by stating that it received and stored these messages.

### Inspecting the SQLite3 Database

- To open the historian database, in the same folder where the historian program is running:
  ```bash
  sqlite3 historian_data.db
  ```

- Within the SQLite3 shell, use the following commands to interact with your database:
  ```sql
  .tables
  ```
  You should see the table that contains the data we are using, `historian_data`.

- To see the command used to create this table, use the command

  ```sql
  .schema historian_data
  ```
  The output should be `CREATE TABLE historian_data (topic TEXT, payload TEXT, timestamp TEXT);`
  This means you should expect this table to contain a topic, a payload, and a timestamp.

- To see the data, use the following command

  ```sql
  select * from historian_data;
  ```

It should output all the data received and stored by the historian program.

- To enhance the output format and make it easier to read, run the commands that follow:

  ```sql
  .mode column
  .headers on
  ```

- Again, to see the data, use the following command

  ```sql
  select * from historian_data;
  ```

  The output should be formatted in columns that are easier to read.

