// ble_sensor_module.cpp
#include "ble_sensor_module.h"
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
BLEServer *pServer;

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"  // Example Service UUID
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"  // Example Characteristic UUID

#define LED_PIN 2  // GPIO pin for the LED (usually GPIO 2 on ESP32)

// Define the custom callback class
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    // Restart advertising to allow reconnections
    pServer->getAdvertising()->start();
    Serial.println("Client disconnected. Advertising restarted.");
  }
};

void setupBLE() {
  Serial.begin(115200);          // Initialize serial communication for debugging
  pinMode(LED_PIN, OUTPUT);      // Set LED pin as output
  BLEDevice::init("ESP32_GATT_Server");

  // Create BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks()); // Use the callback class defined above

  // Create BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
                    );

  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->start();

  Serial.println("BLE GATT Server started. Waiting for a client to connect...");
}

void sendDataAsJson(float temperature, float humidity, int pressure) {
  String jsonData = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + ", \"pressure\": " + String(pressure) + "}";
  pCharacteristic->setValue(jsonData.c_str());
  pCharacteristic->notify();       // Notify clients of the new value
}

void sendDataAsCsv(float temperature, float humidity, int pressure) {
  String csvData = String(temperature) + "," + String(humidity) + "," + String(pressure);
  pCharacteristic->setValue(csvData.c_str());
  pCharacteristic->notify();       // Notify clients of the new value
}

void loopBLE() {
  if (deviceConnected) {
    // Blink LED fast when connected
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);

    // Example sensor data
    float temperature = 25.3;
    float humidity = 60.1;
    int pressure = 1013;

    // Send data in JSON format
    sendDataAsJson(temperature, humidity, pressure);

    // Optionally, send data in CSV format
    // sendDataAsCsv(temperature, humidity, pressure);

    delay(10000);                     // Delay to send data every 10 seconds
  } else {
    // Turn off LED when not connected
    digitalWrite(LED_PIN, LOW);
  }

  // Check for new connection requests
  if (!deviceConnected && pServer->getConnectedCount() > 0) {
    Serial.println("New client connected, switching connection.");
    pServer->disconnect(0);  // Disconnect current client to allow a new connection
  }
}
