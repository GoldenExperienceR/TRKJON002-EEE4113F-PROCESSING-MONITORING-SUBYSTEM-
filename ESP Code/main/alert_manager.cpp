#include "alert_manager.h"

#include "system_data.h"

#include "firmware_config.h"

#include <Arduino.h>

void AlertManager_Init(void)
{
}

void AlertManager_Update(void)
{
    if(SystemData.environment.temperature > TEMP_ALERT_THRESHOLD)
    {
        SystemData.alerts.temperatureAlert = true;
    }
    else if(SystemData.environment.temperature < TEMP_CLEAR_THRESHOLD)
    {
        SystemData.alerts.temperatureAlert = false;
    }
}