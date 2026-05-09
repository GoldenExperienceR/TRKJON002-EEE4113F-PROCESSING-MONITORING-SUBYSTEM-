#ifndef TELEMETRY_TYPES_H
#define TELEMETRY_TYPES_H

#include <Arduino.h>

typedef struct
{
    // =====================================================
    // SYSTEM
    // =====================================================

    uint32_t timestamp_ms;

    uint32_t systemHealthFlags;

    // =====================================================
    // ENVIRONMENTAL SENSOR
    // =====================================================

    float ambientTemp_C;

    float humidity_percent;

    // =====================================================
    // THERMAL SENSORS
    // =====================================================

    float ds18b20Temp_C;

    float ntc1Temp_C;

    float ntc2Temp_C;

    // =====================================================
    // POWER MONITOR
    // =====================================================

    float busVoltage_V;

    float current_mA;

    float power_mW;

    // =====================================================
    // IMU — ACCELEROMETER
    // =====================================================

    float accelX_g;

    float accelY_g;

    float accelZ_g;

    // =====================================================
    // IMU — GYROSCOPE
    // =====================================================

    float gyroX_dps;

    float gyroY_dps;

    float gyroZ_dps;

} TelemetryPacket_t;

#endif