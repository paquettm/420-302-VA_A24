#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <Wire.h>
//WIFI_SSID, WIFI_PASSWORD, MQTT_SERVER, MQTT_PORT
#include "credentials.h"

// MCP9808 constants
#define MCP9808_ADDR_1 0x18    // I2C address of MCP9808 1
#define TEMP_REG 0x05        // Temperature register address

//MQTT client name prefix (will add MAC address)
String name = "ESP32Client_";

// Create an instance of the WiFiClient class
WiFiClient espClient;
// Create an instance of the PubSubClient class
PubSubClient client(espClient);

// Timer for publishing every 5 seconds
unsigned long previousMillis = 0;
const long interval = 10000;

void setup() {
  // Start Serial communication
  Serial.begin(115200);              

  Wire.begin();
  
// Configure MCP9808
  Wire.beginTransmission(MCP9808_ADDR_1);
  Wire.write(0x01);          // Config register
  Wire.write(0x00);          // Clear config
  Wire.write(0x00);
  Wire.endTransmission();
  
 // Read the MAC address
  uint8_t mac[6];
  esp_read_mac(mac, ESP_MAC_WIFI_STA);
  // Convert MAC address to a string
  char macStr[18]; // MAC address is 12 characters long without separators, plus null terminator
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  // Concatenate the name prefix with the MAC address 
  name = name + macStr;

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Set MQTT server and port
  client.setServer(MQTT_SERVER, MQTT_PORT);
}


float readTemperature(int addr) {
  uint8_t data[2];
  
  // Request temperature data
  Wire.beginTransmission(addr);
  Wire.write(TEMP_REG);
  Wire.endTransmission();
  
  // Read 2 bytes of temperature data
  Wire.requestFrom(addr, 2);
  if (Wire.available() == 2) {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
  
  // Convert raw data to temperature
  int16_t temp = ((data[0] & 0x0F) << 8) | data[1];
  float celsius = temp * 0.0625;
  return celsius;
}

void loop() {
  // Connect to MQTT if necessary
  if (!client.connected()) {
    connect();
  }

  // Get the current time
  unsigned long currentMillis = millis();

  // Publish a message every preset interval
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float temperature = readTemperature(MCP9808_ADDR_1);
    char tempString[8];
    dtostrf(temperature, 1, 2, tempString);
    
    client.publish(MCP9808_1_TOPIC, tempString);
    
    Serial.print(MCP9808_1_TOPIC);
    Serial.print(": ");
    Serial.print(tempString);
    Serial.println(" °C");

  }

  // Allow the PubSubClient to process incoming messages
  client.loop();
}

void connect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");

    // Attempt to connect
    if (client.connect(name.c_str())) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.print(client.state());
      Serial.println("Try again in 5 seconds");
      delay(5000);
    }
  }
}