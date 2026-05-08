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


#endif