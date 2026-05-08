#include "storage_manager.h"

#include "firmware_config.h"

#include <Arduino.h>

static uint32_t lastStorageTime = 0;

void StorageManager_Init(void)
{
    Serial.println("Storage Manager Initialized");
}

void StorageManager_Update(void)
{
    uint32_t currentTime = millis();

    if(currentTime - lastStorageTime >= STORAGE_INTERVAL_MS)
    {
        lastStorageTime = currentTime;

        Serial.println("Storage Event");
    }
}