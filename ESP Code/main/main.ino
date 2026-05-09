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

// Comm data types 
#include "telemetry_types.h"


void setup()
{
    Serial.begin(115200);

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
}

void loop()
{
    // SensorManager_Update();

    // AlertManager_Update();

    // CommunicationManager_Update();

    // StorageManager_Update();

    // FaultManager_Update();

    // StateMachine_Update();

     TelemetryPacket_t telemetryData;


    telemetryData.timestamp_ms = millis();

    telemetryData.ambientTemp_C = 24.7;
    telemetryData.humidity_percent = 58.2;

    telemetryData.ds18b20Temp_C = 25.1;

    telemetryData.ntc1Temp_C = 25.3;
    telemetryData.ntc2Temp_C = 24.9;

    telemetryData.busVoltage_V = 12.1;
    telemetryData.current_mA = 350.0;
    telemetryData.power_mW = 4235.0;

    UART_PROTOCOL_SendTelemetry(&telemetryData);

    delay(1000);
}

