#include <Wire.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>

Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();

void setup()
{
    Serial.begin(115200);

    delay(2000);

    Wire.begin(21, 22);

    Serial.println("Initializing LSM9DS1...");

    if (!lsm.begin())
    {
        Serial.println("ERROR: LSM9DS1 not detected");

        while (1);
    }

    Serial.println("LSM9DS1 detected!");

    lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);

    lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);

    lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);

    Serial.println("IMU configured");
}

void loop()
{
    lsm.read();

    sensors_event_t accel;
    sensors_event_t mag;
    sensors_event_t gyro;
    sensors_event_t temp;

    lsm.getEvent(&accel, &mag, &gyro, &temp);

    Serial.println("================================");

    Serial.print("Accel X: ");
    Serial.println(accel.acceleration.x);

    Serial.print("Accel Y: ");
    Serial.println(accel.acceleration.y);

    Serial.print("Accel Z: ");
    Serial.println(accel.acceleration.z);

    Serial.print("Gyro X: ");
    Serial.println(gyro.gyro.x);

    Serial.print("Gyro Y: ");
    Serial.println(gyro.gyro.y);

    Serial.print("Gyro Z: ");
    Serial.println(gyro.gyro.z);

    delay(1000);
}
