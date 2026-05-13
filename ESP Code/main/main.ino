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


float temperature = 0.0f;
float humidity = 0.0f;
float temperature2 = 0.0f;

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

    Serial.println("Initializing SHTC3...");

    if (SHTC3_Init())
    {
        Serial.println("SHTC3 Initialization Successful");
    }
    else
    {
        Serial.println("SHTC3 Initialization Failed");
    }

}

void loop()
{
    

    //StateMachine_Update();
    //WatchdogManager_Reset();

 bool status = SHTC3_ReadData(&temperature, &humidity);

    if (status)
    {
        Serial.println("----- Sensor Reading -----");

        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println(" C");

        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.println(" %");

        Serial.println();
    }
    else
    {
        Serial.println("Sensor Read Failed");
    }

    Serial.println("---Temp Sensor Check ---");
    NTC_ReadTemperature1(&temperature2);
    Serial.print(temperature2);
    


    delay(5000);
     
}

