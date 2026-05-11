#include "sensor_manager.h"

#include "telemetry_types.h"

#include "ds18b20_driver.h"
#include "ntc_driver.h"
#include "ina219_driver.h"
#include "shtc3_driver.h"
#include "mpu6050_driver.h"
#include "rtc_driver.h"

#include <Arduino.h>
#include <string.h>




// =====================================================
// PRIVATE TELEMETRY STORAGE
// =====================================================

static TelemetryPacket_t telemetryData;

// =====================================================
// INIT
// =====================================================

void SensorManager_Init(void)
{
    memset(&telemetryData,
           0,
           sizeof(TelemetryPacket_t));
}

// =====================================================
// UPDATE
// =====================================================

void SensorManager_Update(void)
{
    telemetryData.timestamp_ms = millis();

    telemetryData.systemHealthFlags = 0;

    if (!RTC_GetTimestamp(telemetryData.rtcTimestamp)) { 
        telemetryData.systemHealthFlags |= HEALTH_RTC_FAULT; }


    // =================================================
    // DS18B20
    // =================================================

    if (DS18B20_ReadTemperature(
        &telemetryData.ds18b20Temp_C))
    {
        // Success
    }
    else
    {
        telemetryData.systemHealthFlags |=
            HEALTH_DS18B20_FAULT;
    }

    // =================================================
    // NTC1
    // =================================================

    if (NTC_ReadTemperature1(
        &telemetryData.ntc1Temp_C))
    {
        // Success
    }
    else
    {
        telemetryData.systemHealthFlags |=
            HEALTH_NTC1_FAULT;
    }

    // =================================================
    // NTC2
    // =================================================

    if (NTC_ReadTemperature2(
        &telemetryData.ntc2Temp_C))
    {
        // Success
    }
    else
    {
        telemetryData.systemHealthFlags |=
            HEALTH_NTC2_FAULT;
    }

    // =================================================
    // SHTC3
    // =================================================

    if (SHTC3_ReadData(
        &telemetryData.ambientTemp_C,
        &telemetryData.humidity_percent))
    {
        // Success
    }
    else
    {
        telemetryData.systemHealthFlags |=
            HEALTH_SHTC3_FAULT;
    }

    // =================================================
    // INA219
    // =================================================

    float shuntVoltage;

    float loadVoltage;

    if (INA219_ReadData(
        &telemetryData.busVoltage_V,
        &shuntVoltage,
        &loadVoltage,
        &telemetryData.current_mA,
        &telemetryData.power_mW))
    {
        // Success
    }
    else
    {
        telemetryData.systemHealthFlags |=
            HEALTH_INA219_FAULT;
    }

    // =================================================
    // MPU6050
    // =================================================

    if (MPU6050_ReadMotionData(
        &telemetryData.accelX_g,
        &telemetryData.accelY_g,
        &telemetryData.accelZ_g,
        &telemetryData.gyroX_dps,
        &telemetryData.gyroY_dps,
        &telemetryData.gyroZ_dps))
    {
        // Success
    }
    else
    {
        telemetryData.systemHealthFlags |=
            HEALTH_MPU6050_FAULT;
    }
}

// =====================================================
// GET TELEMETRY
// =====================================================

TelemetryPacket_t* SensorManager_GetTelemetry(void)
{
    return &telemetryData;
}

