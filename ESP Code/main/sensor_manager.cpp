#include "sensor_manager.h"

#include "firmware_config.h"

#include <Arduino.h>

static uint32_t lastSensorPollTime = 0;

void SensorManager_Init(void)
{
    Serial.println("Sensor Manager Initialized");
}

void SensorManager_Update(void)
{
    uint32_t currentTime = millis();

    if(currentTime - lastSensorPollTime >= SENSOR_POLL_INTERVAL_MS)
    {
        lastSensorPollTime = currentTime;

        Serial.println("Polling Sensors");
    }
}