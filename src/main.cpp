#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <data/fs/file_system.h>
#include <ESP32Time.h>

#define FORMAT_LITTLEFS_IF_FAILED true
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

ESP32Time rtc(3600);

class MyBLEServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    Serial.println("Someone connected to us:");
    Serial.println(pServer->getConnectedCount());
  };

  void onDisconnect(BLEServer *pServer)
  {
    Serial.println("Someone disconnected to us:");
    Serial.println(pServer->getConnectedCount());
    pServer->startAdvertising();
  }
};

class MyCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    uint8_t *value = pCharacteristic->getData();
    size_t length = pCharacteristic->getLength();

    // Check if the data matches "DELETE"
    if (length == 6 && strncmp((char *)value, "DELETE", 6) == 0)
    {
      Serial.println("Received DELETE command!");
      FileSystem::cleanFile();
      return;
    }

    if (*value > 0)
    {
      bool success = FileSystem::writeToFile(value, length);
      if (success)
      {
        Serial.println("Value successfully written!");
      }
      else
      {
        Serial.println("Value failed to write!");
      }
    }
  }

  void onRead(BLECharacteristic *pCharacteristic, esp_ble_gatts_cb_param_t *param)
  {
    String value = FileSystem::readFromFile();

    Serial.println(value);
  }
};

void setup()
{
  Serial.begin(9600);

  int year = rtc.getYear();
  if (year == 1970)
  {
    rtc.setTime(0, 0, 18, 8, 12, 2021); // 17th Jan 2021 15:24:30
  }

  if (!FileSystem::spiffsAvailable())
  {
    Serial.println("SPIFFS not available, extiting...");
    return;
  }

  BLEDevice::init("MyESP32");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyBLEServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();

  pServer->startAdvertising();
}

void loop()
{
  delay(2000);
  Serial.println(rtc.getDateTime());
}