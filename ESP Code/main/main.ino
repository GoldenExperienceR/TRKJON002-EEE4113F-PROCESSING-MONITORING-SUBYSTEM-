// Architecture Libraries
#include "firmware_types.h"
#include "firmware_config.h"
#include "state_machine.h"
#include "sensor_manager.h"
#include "alert_manager.h"
#include "communication_manager.h"
#include "storage_manager.h"
#include "fault_manager.h"

//Driver Libraries
#include "rtc_driver.h"
#include "sd_driver.h"
#include "shtc3_driver.h"
#include "mpu6050_driver.h"
#include "ina219_driver.h"
#include "ds18b20_driver.h"
#include "ntc_driver.h"
#include "uart_protocol.h"

// Telemetry Struct 
#include "telemetry_types.h"

//Watchdog
//#include "watchdog_manager.h"



float accelX = 0.0f;
float accelY = 0.0f;
float accelZ = 0.0f;

float gyroX = 0.0f;
float gyroY = 0.0f;
float gyroZ = 0.0f;

void setup()
{
    Serial.begin(115200);

    Serial.println("SYSTEM BOOT");

    //Manager Intialisiations 
    SensorManager_Init();
    AlertManager_Init();
    CommunicationManager_Init();
    StorageManager_Init();
    FaultManager_Init();
    StateMachine_Init();

    // Driver Intializations
    RTC_Init();
    SD_Init();
    SHTC3_Init();
    MPU6050_Init();
    INA219_Init();
    DS18B20_Init();
    NTC_Init();
    UART_PROTOCOL_Init();

    // Additional 
    //WatchdogManager_Init();

    delay(1000);

    Serial.println("Initializing MPU6050...");

    if (MPU6050_Init())
    {
        Serial.println("MPU6050 Initialization Successful");
    }
    else
    {
        Serial.println("MPU6050 Initialization Failed");
    }

}

void loop()
{
    

    //StateMachine_Update();
    //WatchdogManager_Reset();

  bool status = MPU6050_ReadMotionData(
        &accelX,
        &accelY,
        &accelZ,
        &gyroX,
        &gyroY,
        &gyroZ
    );

    if (status)
    {
        Serial.println("===== MPU6050 Data =====");

        Serial.print("Accel X: ");
        Serial.println(accelX);

        Serial.print("Accel Y: ");
        Serial.println(accelY);

        Serial.print("Accel Z: ");
        Serial.println(accelZ);

        Serial.print("Gyro X: ");
        Serial.println(gyroX);

        Serial.print("Gyro Y: ");
        Serial.println(gyroY);

        Serial.print("Gyro Z: ");
        Serial.println(gyroZ);

        Serial.println();
    }
    else
    {
        Serial.println("MPU6050 Read Failed");
    }

    delay(500);
     
}

