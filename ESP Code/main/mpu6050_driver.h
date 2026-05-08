#ifndef MPU6050_DRIVER_H
#define MPU6050_DRIVER_H

bool MPU6050_Init(void);

bool MPU6050_ReadMotionData(
    float* accelX,
    float* accelY,
    float* accelZ,
    float* gyroX,
    float* gyroY,
    float* gyroZ
);

#endif