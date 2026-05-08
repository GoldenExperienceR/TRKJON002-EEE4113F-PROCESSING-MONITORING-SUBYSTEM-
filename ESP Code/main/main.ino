// Architecture Libraries
#include "firmware_types.h"
#include "firmware_config.h"
#include "state_machine.h"
#include "sensor_manager.h"
#include "alert_manager.h"
#include "communication_manager.h"
#include "storage_manager.h"
#include "fault_manager.h"

//Driver Libraries
#include "rtc_driver.h"
#include "sd_driver.h"

void setup()
{
    Serial.begin(115200);

    //Manager Intialisiations 
    SensorManager_Init();
    AlertManager_Init();
    CommunicationManager_Init();
    StorageManager_Init();
    FaultManager_Init();
    StateMachine_Init();

    // Driver Intializations
    RTC_Init();
    SD_Init();
}

void loop()
{
    SensorManager_Update();

    AlertManager_Update();

    CommunicationManager_Update();

    StorageManager_Update();

    FaultManager_Update();

    StateMachine_Update();

    char timestamp[25];

char logEntry[100];

RTC_GetTimestamp(timestamp);

sprintf(
    logEntry,
    "%s,%.2f,%.2f",
    timestamp,
    25.4,
    48.1
);

SD_WriteLogEntry(logEntry);

Serial.println(logEntry);

delay(2000);

}