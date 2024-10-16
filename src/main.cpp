#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;

#define SERVICE_UUID        "91bad492-b950-4226-aa2b-4ede9fa42f59"
#define CHARACTERISTIC_UUID "cba1d466-344c-4be3-ab3f-189f80dd7518"

void setup() {
  Serial.begin(115200);

  // Créer le périphérique BLE
  BLEDevice::init("ESP32_BLE_Ashraf");

  // Créer un serveur BLE
  pServer = BLEDevice::createServer();

  // Créer un service BLE
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Créer une caractéristique BLE
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE
                    );

  // Créer un descripteur de la caractéristique
  pCharacteristic->addDescriptor(new BLE2902());

  // Démarrer le service
  pService->start();

  // Commencer à faire de la publicité BLE
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->start();

  Serial.println("BLE Server started, advertising...");
}

void loop() {
  // Le code du serveur BLE continuera à tourner ici
}
