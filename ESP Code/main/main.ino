#include "firmware_types.h"
#include "firmware_config.h"

#include "state_machine.h"
#include "sensor_manager.h"
#include "alert_manager.h"
#include "communication_manager.h"
#include "storage_manager.h"
#include "fault_manager.h"

void setup()
{
    Serial.begin(115200);

    SensorManager_Init();
    AlertManager_Init();
    CommunicationManager_Init();
    StorageManager_Init();
    FaultManager_Init();

    StateMachine_Init();
}

void loop()
{
    SensorManager_Update();

    AlertManager_Update();

    CommunicationManager_Update();

    StorageManager_Update();

    FaultManager_Update();

    StateMachine_Update();
}