#include "sd_driver.h"

#include <SPI.h>
#include <SD.h>

#define SD_CS_PIN 5

static bool sdInitialized = false;

/*====================================
  SD CARD INITIALIZATION
====================================*/

bool SD_Init(void)
{
    if(!SD.begin(SD_CS_PIN))
    {
        Serial.println("SD Initialization Failed");

        sdInitialized = false;

        return false;
    }

    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE)
    {
        Serial.println("No SD Card Detected");

        sdInitialized = false;

        return false;
    }

    // =====================================================
    // CREATE CSV FILE WITH HEADER IF IT DOES NOT EXIST
    // =====================================================

    if(!SD.exists("/system_log.csv"))
    {
        File logFile =
            SD.open("/system_log.csv", FILE_WRITE);

        if(logFile)
        {
            logFile.println(
                "RTC_Time,"
                "Millis,"
                "State,"
                "HealthFlags,"
                "AmbientTemp_C,"
                "Humidity_percent,"
                "DS18B20Temp_C,"
                "NTC1Temp_C,"
                "NTC2Temp_C,"
                "BusVoltage_V,"
                "Current_mA,"
                "Power_mW,"
                "AccelX_g,"
                "AccelY_g,"
                "AccelZ_g,"
                "GyroX_dps,"
                "GyroY_dps,"
                "GyroZ_dps"
            );

            logFile.close();

            Serial.println("CSV Header Created");
        }
    }

    Serial.println("SD Card Initialized");

    sdInitialized = true;

    return true;
}



/*====================================
  WRITE LOG ENTRY
====================================*/

bool SD_WriteLogEntry(const char* logEntry)
{
    if(!sdInitialized)
    {
        return false;
    }

    File logFile = SD.open("/system_log.csv", FILE_APPEND);

    if(!logFile)
    {
        Serial.println("Failed To Open Log File");

        return false;
    }

    logFile.println(logEntry);

    logFile.close();

    return true;
}