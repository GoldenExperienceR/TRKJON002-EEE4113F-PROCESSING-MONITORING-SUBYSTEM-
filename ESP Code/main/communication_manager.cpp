#include "communication_manager.h"

#include "firmware_config.h"
#include "state_machine.h"

#include <Arduino.h>

static uint32_t lastTXTime = 0;

void CommunicationManager_Init(void)
{
    Serial.println("Communication Manager Initialized");
}

void CommunicationManager_Update(void)
{
    uint32_t currentTime = millis();

    SystemState_t currentState = StateMachine_GetState();

    switch(currentState)
    {
        case SYS_NORMAL:

            if(currentTime - lastTXTime >= NORMAL_TX_INTERVAL_MS)
            {
                lastTXTime = currentTime;

                Serial.println("NORMAL TRANSMISSION");
            }

            break;

        case SYS_ALERT:

            if(currentTime - lastTXTime >= ALERT_TX_INTERVAL_MS)
            {
                lastTXTime = currentTime;

                Serial.println("HAZARD TRANSMISSION");
            }

            break;

        default:
            break;
    }
}