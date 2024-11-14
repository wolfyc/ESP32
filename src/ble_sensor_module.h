// ble_sensor_module.h
#ifndef BLE_SENSOR_MODULE_H
#define BLE_SENSOR_MODULE_H

#include <BLEDevice.h>
#include <BLEServer.h>

void setupBLE();
void loopBLE();
void sendDataAsJson(float temperature, float humidity, int pressure);
void sendDataAsCsv(float temperature, float humidity, int pressure);

#endif // BLE_SENSOR_MODULE_H
