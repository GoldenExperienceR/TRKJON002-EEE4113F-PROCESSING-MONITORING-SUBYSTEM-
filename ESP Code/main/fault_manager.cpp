#include "fault_manager.h"

#include <Arduino.h>

#include "sensor_manager.h"
#include "telemetry_types.h"
#include "system_data.h"



static bool criticalFaultActive = false;

/*==================================================
INITIALIZATION
==================================================*/

void FaultManager_Init(void)
{
    criticalFaultActive = false;
}

/*==================================================
FAULT SUPERVISION
==================================================*/

void FaultManager_Update(void)
{
    TelemetryPacket_t* telemetry =
        SensorManager_GetTelemetry();

    uint32_t flags =
        telemetry->systemHealthFlags;

    criticalFaultActive = false;

    // ==============================================
    // WARNING FAULTS
    // ==============================================

    if(flags & HEALTH_SHTC3_FAULT)
    {
        Serial.println(
            "FAULT: SHTC3 FAILURE");
    }

    if(flags & HEALTH_DS18B20_FAULT)
    {
        Serial.println(
            "FAULT: DS18B20 FAILURE");
    }

    if(flags & HEALTH_NTC1_FAULT)
    {
        Serial.println(
            "FAULT: NTC1 FAILURE");
    }

    if(flags & HEALTH_NTC2_FAULT)
    {
        Serial.println(
            "FAULT: NTC2 FAILURE");
    }

    if(flags & HEALTH_INA219_FAULT)
    {
        Serial.println(
            "FAULT: INA219 FAILURE");
    }

    if(flags & HEALTH_MPU6050_FAULT)
    {
        Serial.println(
            "FAULT: MPU6050 FAILURE");
    }

    if(flags & HEALTH_SD_FAULT)
    {
        Serial.println(
            "FAULT: SD FAILURE");
    }

    if(flags & HEALTH_UART_FAULT)
    {
        Serial.println(
            "FAULT: UART FAILURE");
    }

    if(flags & HEALTH_RTC_FAULT)
    {
        Serial.println(
            "FAULT: RTC FAILURE");
    }

    if(backupPowerEventTriggered)
    {
        backupPowerEventTriggered = false;

        telemetry->systemHealthFlags|= HEALTH_BACKUP_POWER_ACTIVE;

        Serial.println( "BACKUP POWER ACTIVE");

    }



    // ==============================================
    // CRITICAL CONDITIONS
    // ==============================================

    bool majorSensorFailure =
        (flags & HEALTH_DS18B20_FAULT) &&
        (flags & HEALTH_SHTC3_FAULT) &&
        (flags & HEALTH_INA219_FAULT);

    bool dataSystemFailure =
        (flags & HEALTH_SD_FAULT) &&
        (flags & HEALTH_UART_FAULT);

    // ==============================================
    // CRITICAL DECISION
    // ==============================================

    if(majorSensorFailure)
    {
        Serial.println(
            "CRITICAL: SENSOR SYSTEM FAILURE");

        criticalFaultActive = true;
    }

    if(dataSystemFailure)
    {
        Serial.println(
            "CRITICAL: DATA SYSTEM FAILURE");

        criticalFaultActive = true;
    }

    // ==============================================
    // UPDATE GLOBAL FLAG
    // ==============================================

    if(criticalFaultActive)
    {
        telemetry->systemHealthFlags |=
            HEALTH_CRITICAL_FAULT;
    }
    else
    {
        telemetry->systemHealthFlags &=
            ~HEALTH_CRITICAL_FAULT;
    }
}

/*==================================================
CRITICAL STATUS
==================================================*/

bool FaultManager_HasCriticalFault(void)
{
    return criticalFaultActive;
}

