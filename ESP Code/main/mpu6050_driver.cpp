#include "mpu6050_driver.h"

#include <Arduino.h>

static bool mpuInitialized = false;

/*====================================
  INITIALIZATION
====================================*/

bool MPU6050_Init(void)
{
    Serial.println("Mock MPU6050 Initialized");

    mpuInitialized = true;

    return true;
}

/*====================================
  READ MOTION DATA
====================================*/

bool MPU6050_ReadMotionData(
    float* accelX,
    float* accelY,
    float* accelZ,
    float* gyroX,
    float* gyroY,
    float* gyroZ
)
{
    if(!mpuInitialized)
    {
        return false;
    }

    /*
      Simulated accelerometer data
    */

    *accelX = random(-100, 100) * 0.01;

    *accelY = random(-100, 100) * 0.01;

    *accelZ = 1.0 + random(-20, 20) * 0.01;

    /*
      Simulated gyroscope data
    */

    *gyroX = random(-300, 300) * 0.1;

    *gyroY = random(-300, 300) * 0.1;

    *gyroZ = random(-300, 300) * 0.1;

    return true;
}