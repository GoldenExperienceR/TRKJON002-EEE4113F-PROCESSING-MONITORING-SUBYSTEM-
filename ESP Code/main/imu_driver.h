#ifndef IMU_DRIVER_H
#define IMU_DRIVER_H

#include <Arduino.h>

bool IMU_Init(void);

bool IMU_ReadMotionData(
    float* accelX,
    float* accelY,
    float* accelZ,
    float* gyroX,
    float* gyroY,
    float* gyroZ
);

#endif