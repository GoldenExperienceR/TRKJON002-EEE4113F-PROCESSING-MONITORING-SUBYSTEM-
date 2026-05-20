#include "imu_driver.h"

#include <Wire.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>

static Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();

bool IMU_Init(void)
{
    Wire.begin(21, 22);

    if (!lsm.begin())
    {
        return false;
    }

    lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);

    lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);

    lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);

    return true;
}

bool IMU_ReadMotionData(
    float* accelX,
    float* accelY,
    float* accelZ,
    float* gyroX,
    float* gyroY,
    float* gyroZ
)
{
    sensors_event_t accel;
    sensors_event_t mag;
    sensors_event_t gyro;
    sensors_event_t temp;

    lsm.getEvent(&accel, &mag, &gyro, &temp);

    *accelX = accel.acceleration.x;
    *accelY = accel.acceleration.y;
    *accelZ = accel.acceleration.z;

    // Convert rad/s -> deg/s
    *gyroX = gyro.gyro.x * 180.0 / PI;
    *gyroY = gyro.gyro.y * 180.0 / PI;
    *gyroZ = gyro.gyro.z * 180.0 / PI;

    return true;
}
