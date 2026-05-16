#include "state_machine.h"

#include <Arduino.h>

#include "alert_manager.h"
#include "fault_manager.h"
#include "storage_manager.h"
#include "communication_manager.h"
#include "sensor_manager.h"
#include "firmware_config.h"
//#include "watchdog_manager.h"


static SystemState_t currentState;
static uint32_t lastIdleEntryTime = 0;
static uint32_t idleStartTime = 0;





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

         SensorManager_Update();
         AlertManager_Update(); 
         FaultManager_Update(); 
         CommunicationManager_Update(); 
         StorageManager_Update();

        if(FaultManager_HasCriticalFault())
        {
            Serial.println(
                "NORMAL -> ERROR");

            currentState = SYS_ERROR;
        }
        else if(millis() - lastIdleEntryTime >=
                IDLE_ENTRY_INTERVAL_MS)
        {
            Serial.println(
                "NORMAL -> IDLE");

            idleStartTime = millis();

            lastIdleEntryTime = millis();

            currentState = SYS_IDLE;
        }
        
            break;


        // =================================================
        // ALERT
        // =================================================

        case SYS_ALERT:
        {
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
            SensorManager_Update(); 
            CommunicationManager_Update(); 
            StorageManager_Update(); 
            FaultManager_Update(); 
            if(!FaultManager_HasCriticalFault()) {
                 Serial.println( "ERROR -> NORMAL");
                  currentState = SYS_NORMAL; }
            break;
        }

        // =================================================
        // IDLE
        // =================================================

       
       
        case SYS_IDLE:

           // WatchdogManager_Reset();
            if(millis() - idleStartTime >=
                IDLE_DURATION_MS)
            {
                Serial.println(
                    "IDLE -> NORMAL");

                currentState = SYS_NORMAL;
            }

            break;





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
