#ifndef FIRMWARE_TYPES_H
#define FIRMWARE_TYPES_H

#include <Arduino.h>

/*====================================
  SYSTEM STATES
====================================*/

typedef enum
{
    SYS_INIT,
    SYS_NORMAL,
    SYS_ALERT,
    SYS_ERROR,
    SYS_IDLE

} SystemState_t;


/*====================================
  GENERIC STATUS CODES
====================================*/

typedef enum
{
    FW_OK,
    FW_ERROR,
    FW_TIMEOUT,
    FW_INVALID_DATA,
    FW_NOT_INITIALIZED,
    FW_DEVICE_MISSING

} FW_Status_t;


/*====================================
  SENSOR HEALTH
====================================*/

typedef struct
{
    bool initialized;
    bool responding;
    bool valid;

    uint32_t lastUpdateTime;

    uint32_t failureCount;

} SensorHealth_t;

/*====================================
  ENVIRONMENTAL SENSOR DATA
====================================*/

typedef struct
{
    float temperature;

    float humidity;

    SensorHealth_t health;

} EnvironmentalData_t;

/*====================================
  MOTION SENSOR DATA
====================================*/

typedef struct
{
    float accelX;

    float accelY;

    float accelZ;

    float gyroX;

    float gyroY;

    float gyroZ;

    float tiltAngle;

    float shockMagnitude;

    SensorHealth_t health;

} MotionData_t;

/*====================================
  POWER SENSOR DATA
====================================*/

typedef struct
{
    float busVoltage;

    float shuntVoltage;

    float loadVoltage;

    float current;

    float power;

    SensorHealth_t health;

} PowerData_t;
/*====================================
  ALERT STATUS
====================================*/

typedef struct
{
    bool temperatureAlert;

    bool voltageAlert;

    bool shockAlert;

    bool tiltAlert;

    bool powerStateAlert;

    bool systemAlertActive;

} AlertStatus_t;


/*====================================
  GLOBAL SYSTEM DATA
====================================*/

typedef struct
{
    EnvironmentalData_t environment;

    MotionData_t motion;

    PowerData_t battery;

    AlertStatus_t alerts;

} SystemData_t;


#endif