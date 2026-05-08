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