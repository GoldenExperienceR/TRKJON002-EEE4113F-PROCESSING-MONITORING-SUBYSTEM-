#include "communication_manager.h"

#include "firmware_config.h"

#include <Arduino.h>

static uint32_t lastTXTime = 0;

void CommunicationManager_Init(void)
{
    Serial.println("Communication Manager Initialized");
}

void CommunicationManager_Update(void)
{
    uint32_t currentTime = millis();

    if(currentTime - lastTXTime >= NORMAL_TX_INTERVAL_MS)
    {
        lastTXTime = currentTime;

        Serial.println("Normal Transmission Event");
    }
}