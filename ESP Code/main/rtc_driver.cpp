#include "rtc_driver.h"

#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

/*====================================
  RTC INITIALIZATION
====================================*/

bool RTC_Init(void)
{
    Wire.begin();

    if(!rtc.begin())
    {
        Serial.println("RTC Initialization Failed");

        return false;
    }

    Serial.println("RTC Initialized");

    return true;
}

/*====================================
  GET FORMATTED TIMESTAMP
====================================*/

bool RTC_GetTimestamp(char* timestampBuffer)
{
    DateTime now = rtc.now();

    sprintf(
        timestampBuffer,
        "%04d/%02d/%02d %02d:%02d:%02d",
        now.year(),
        now.month(),
        now.day(),
        now.hour(),
        now.minute(),
        now.second()
    );

    return true;
}