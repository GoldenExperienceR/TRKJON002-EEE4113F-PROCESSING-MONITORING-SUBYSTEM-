#ifndef TELEMETRY_TYPES_H
#define TELEMETRY_TYPES_H

#include <Arduino.h>

// =====================================================
// SYSTEM HEALTH FLAG DEFINITIONS
// =====================================================

#define HEALTH_DS18B20_FAULT   (1 << 0)

#define HEALTH_NTC1_FAULT      (1 << 1)

#define HEALTH_NTC2_FAULT      (1 << 2)

#define HEALTH_SHTC3_FAULT     (1 << 3)

#define HEALTH_INA219_FAULT    (1 << 4)

#define HEALTH_MPU6050_FAULT   (1 << 5)

#define HEALTH_RTC_FAULT       (1 << 6)

#define HEALTH_SD_FAULT        (1 << 7)

#define HEALTH_UART_FAULT      (1 << 8)

#define HEALTH_CRITICAL_FAULT  (1 << 9)




typedef struct
{
    // =====================================================
    // SYSTEM
    // =====================================================

    uint32_t timestamp_ms;

    char rtcTimestamp[32];

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