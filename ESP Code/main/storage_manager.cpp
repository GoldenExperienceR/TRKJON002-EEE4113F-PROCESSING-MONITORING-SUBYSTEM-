#include "storage_manager.h"

#include <Arduino.h>
#include <stdio.h>

#include "sd_driver.h"
#include "sensor_manager.h"
#include "state_machine.h"
#include "firmware_config.h"

#include "telemetry_types.h"

static uint32_t lastStorageTime = 0;

// =====================================================
// STATE STRING CONVERSION
// =====================================================

static const char* StateToString(SystemState_t state)
{
    switch(state)
    {
        case SYS_INIT:
            return "INIT";

        case SYS_NORMAL:
            return "NORMAL";

        case SYS_ALERT:
            return "ALERT";

        case SYS_ERROR:
            return "ERROR";

        case SYS_IDLE:
            return "IDLE";

        default:
            return "UNKNOWN";
    }
}

// =====================================================
// INITIALIZATION
// =====================================================

void StorageManager_Init(void)
{
    lastStorageTime = 0;
}

// =====================================================
// PERIODIC LOGGING
// =====================================================

void StorageManager_Update(void)
{
    uint32_t currentTime = millis();

    if(currentTime - lastStorageTime >=
       STORAGE_LOG_INTERVAL_MS)
    {
        lastStorageTime = currentTime;

        TelemetryPacket_t* telemetry =
            SensorManager_GetTelemetry();

        char logBuffer[512];

        snprintf(
            logBuffer,
            sizeof(logBuffer),

            "%s,"
            "%lu,"
            "%s,"
            "0x%08lX,"
            "%.2f,"
            "%.2f,"
            "%.2f,"
            "%.2f,"
            "%.2f,"
            "%.2f,"
            "%.2f,"
            "%.2f,"
            "%.3f,"
            "%.3f,"
            "%.3f,"
            "%.3f,"
            "%.3f,"
            "%.3f",

            telemetry->rtcTimestamp,

            telemetry->timestamp_ms,

            StateToString(
                StateMachine_GetState()),

            telemetry->systemHealthFlags,

            telemetry->ambientTemp_C,

            telemetry->humidity_percent,

            telemetry->ds18b20Temp_C,

            telemetry->ntc1Temp_C,

            telemetry->ntc2Temp_C,

            telemetry->busVoltage_V,

            telemetry->current_mA,

            telemetry->power_mW,

            telemetry->accelX_g,

            telemetry->accelY_g,

            telemetry->accelZ_g,

            telemetry->gyroX_dps,

            telemetry->gyroY_dps,

            telemetry->gyroZ_dps
        );

        if(SD_WriteLogEntry(logBuffer))
        {
            Serial.println("Telemetry Logged");
        }
        else
        {
            Serial.println("Storage Write Failed");
            telemetry->systemHealthFlags |= HEALTH_SD_FAULT;
        }
    }
}

