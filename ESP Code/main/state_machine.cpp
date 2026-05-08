#include "state_machine.h"
#include <Arduino.h>

static SystemState_t currentState = SYS_INIT;

void StateMachine_Init(void)
{
    currentState = SYS_INIT;
}

void StateMachine_Update(void)
{
    switch(currentState)
    {
        case SYS_INIT:

            Serial.println("STATE: INIT");

            currentState = SYS_NORMAL;

            break;

        case SYS_NORMAL:

            Serial.println("STATE: NORMAL");

            break;

        case SYS_ALERT:

            Serial.println("STATE: ALERT");

            break;

        case SYS_ERROR:

            Serial.println("STATE: ERROR");

            break;

        case SYS_IDLE:

            Serial.println("STATE: IDLE");

            break;
    }

    delay(1000);
}

SystemState_t StateMachine_GetState(void)
{
    return currentState;
}