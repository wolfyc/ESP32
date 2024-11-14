// main.cpp
#include <Arduino.h>
#include "ble_sensor_module.h"

void setup() {
  setupBLE();  // Initialize BLE setup
}

void loop() {
  // Send dummy sensor data using the BLE functions
  float temperature = 25.3;
  float humidity = 90;
  int pressure = 1013;

  sendDataAsJson(temperature, humidity, pressure);  // Send data in JSON format
  delay(10000);  // Wait for 10 seconds before sending again
}
