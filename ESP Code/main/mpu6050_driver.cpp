#include "MPU6050_Driver.h"

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// =====================================================
// MPU6050 Object
// =====================================================

static Adafruit_MPU6050 mpu;

// =====================================================
// Public Functions
// =====================================================

bool MPU6050_Init(void)
{
    Wire.begin(21, 22);

    if (!mpu.begin(0x68, &Wire))
    {
        return false;
    }

    // Accelerometer Range
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

    // Gyroscope Range
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);

    // Low Pass Filter
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    delay(100);

    return true;
}

bool MPU6050_ReadMotionData(
    float* accelX,
    float* accelY,
    float* accelZ,
    float* gyroX,
    float* gyroY,
    float* gyroZ
)
{
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;

    mpu.getEvent(
        &accel,
        &gyro,
        &temp
    );

    // =============================================
    // Acceleration
    // Convert m/s^2 -> g
    // =============================================

    *accelX =
        accel.acceleration.x / 9.80665f;

    *accelY =
        accel.acceleration.y / 9.80665f;

    *accelZ =
        accel.acceleration.z / 9.80665f;

    // =============================================
    // Gyroscope
    // Convert rad/s -> deg/s
    // =============================================

    *gyroX =
        gyro.gyro.x * RAD_TO_DEG;

    *gyroY =
        gyro.gyro.y * RAD_TO_DEG;

    *gyroZ =
        gyro.gyro.z * RAD_TO_DEG;

    return true;
}