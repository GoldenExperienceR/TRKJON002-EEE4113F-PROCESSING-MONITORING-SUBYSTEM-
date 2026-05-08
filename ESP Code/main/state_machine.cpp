#include "state_machine.h"

#include <Arduino.h>

static SystemState_t currentState = SYS_INIT;

static uint32_t stateStartTime = 0;

void StateMachine_Init(void)
{
    currentState = SYS_INIT;

    stateStartTime = millis();
}

void StateMachine_Update(void)
{
    uint32_t currentTime = millis();

    switch(currentState)
    {
        case SYS_INIT:

            Serial.println("STATE: INIT");

            currentState = SYS_NORMAL;

            stateStartTime = currentTime;

            break;

        case SYS_NORMAL:

            if(currentTime - stateStartTime >= 10000)
            {
                Serial.println("TRANSITION TO ALERT");

                currentState = SYS_ALERT;

                stateStartTime = currentTime;
            }

            break;

        case SYS_ALERT:

            if(currentTime - stateStartTime >= 10000)
            {
                Serial.println("TRANSITION TO NORMAL");

                currentState = SYS_NORMAL;

                stateStartTime = currentTime;
            }

            break;

        default:
            break;
    }
}

SystemState_t StateMachine_GetState(void)
{
    return currentState;
}