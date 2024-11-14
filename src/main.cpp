#include <Arduino.h>            // Include the Arduino core library
#include <BLEDevice.h>          // Include the BLE libraries
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

void setup() {
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

  pCharacteristic->setValue("Hello from ESP32");
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->start();

  Serial.println("BLE GATT Server started. Waiting for a client to connect...");
}

void loop() {
  if (deviceConnected) {
    // Blink LED fast when connected
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);

    // Send a text message via BLE
    const char* message = "Hello, this is a text message from ESP32";
    pCharacteristic->setValue(message);
    pCharacteristic->notify();       // Notify clients of the new value
    delay(1000);                     // Delay to limit notifications
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
