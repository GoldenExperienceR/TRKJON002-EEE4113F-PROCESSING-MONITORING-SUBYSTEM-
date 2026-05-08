#include "sensor_manager.h"

#include "system_data.h"

#include <Arduino.h>

void SensorManager_Init(void)
{
    Serial.println("Sensor Manager Initialized");
}

void SensorManager_Update(void)
{
    static uint32_t lastPoll = 0;

    if(millis() - lastPoll >= 1000)
    {
        lastPoll = millis();

        SystemData.environment.temperature += 1.0;

        Serial.print("Temperature: ");

        Serial.println(SystemData.environment.temperature);
    }
}