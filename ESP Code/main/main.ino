// Architecture Libraries
#include "firmware_types.h"
#include "firmware_config.h"
#include "state_machine.h"
#include "sensor_manager.h"
#include "alert_manager.h"
#include "communication_manager.h"
#include "storage_manager.h"
#include "fault_manager.h"
#include "system_data.h"



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

// Power state interrupt handler
        void IRAM_ATTR PowerState_ISR(void)
        { 
            backupPowerEventTriggered = true;
             Serial.println( "BACKUP POWER ACTIVE");
            
        }


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

    // Watch Dog Timer 
    //WatchdogManager_Init();

    // Interrupt handler 

        pinMode(PWR_STATE_PIN, INPUT_PULLDOWN); // internally pulls pin to ground -> default state is a stable LOW 
        attachInterrupt(
            digitalPinToInterrupt(PWR_STATE_PIN),
            PowerState_ISR,
            RISING);




    
}

void loop()
{
    
    StateMachine_Update();
    //WatchdogManager_Reset(); 

}

