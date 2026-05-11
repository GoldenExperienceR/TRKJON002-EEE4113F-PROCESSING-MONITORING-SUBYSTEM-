#include "state_machine.h"

#include <Arduino.h>

#include "alert_manager.h"
#include "fault_manager.h"
#include "storage_manager.h"
#include "communication_manager.h"
#include "sensor_manager.h"


static SystemState_t currentState;

void StateMachine_Init(void)
{
    currentState = SYS_INIT;
}

void StateMachine_Update(void)
{
    switch(currentState)
    {
        // =================================================
        // INIT
        // =================================================

        case SYS_INIT:
        {
            Serial.println("INIT");
            if(FaultManager_HasCriticalFault())
            {
                currentState = SYS_ERROR;

                Serial.println("STATE -> ERROR");
            }
            else
            {
                currentState = SYS_NORMAL;

                Serial.println("STATE -> NORMAL");
            }

            break;
        }

        // =================================================
        // NORMAL
        // =================================================

        case SYS_NORMAL:
        {
            Serial.println("NORMAL");
            if(FaultManager_HasCriticalFault())
            {
                currentState = SYS_ERROR;

                Serial.println("STATE -> ERROR");
            }
            else if(AlertManager_IsAlertActive())
            {
                currentState = SYS_ALERT;

                Serial.println("STATE -> ALERT");
            }

            break;
        }

        // =================================================
        // ALERT
        // =================================================

        case SYS_ALERT:
        {
            Serial.println("ALERT");
            if(FaultManager_HasCriticalFault())
            {
                currentState = SYS_ERROR;

                Serial.println("STATE -> ERROR");
            }
            else if(!AlertManager_IsAlertActive())
            {
                currentState = SYS_NORMAL;

                Serial.println("STATE -> NORMAL");
            }

            break;
        }

        // =================================================
        // ERROR
        // =================================================

        case SYS_ERROR:
        {
            Serial.println("ERROR");
            SensorManager_Update(); 
            CommunicationManager_Update(); 
            StorageManager_Update(); 
            FaultManager_Update(); 
            if(!FaultManager_HasCriticalFault()) {
                 Serial.println( "STATE TRANSITION: ERROR -> NORMAL");
                  currentState = SYS_NORMAL; }
            break;
        }

        // =================================================
        // IDLE
        // =================================================

        case SYS_IDLE:
        {
            Serial.println("IDLE");

            currentState = SYS_NORMAL;

            Serial.println("STATE -> NORMAL");

            break;
        }

        default:
        {
            currentState = SYS_ERROR;

            break;
        }
    }
}

SystemState_t StateMachine_GetState(void)
{
    return currentState;
}
