#include "shtc3_driver.h"

#include <Arduino.h>

static bool shtc3Initialized = false;

/*====================================
  INITIALIZATION
====================================*/

bool SHTC3_Init(void)
{
    Serial.println("Mock SHTC3 Initialized");

    shtc3Initialized = true;

    return true;
}

/*====================================
  READ SENSOR DATA
====================================*/

bool SHTC3_ReadData(float* temperature, float* humidity)
{
    if(!shtc3Initialized)
    {
        return false;
    }

    /*
      Simulated data for architecture testing
    */

    *temperature = 24.0 + random(-20, 20) * 0.1;

    *humidity = 50.0 + random(-50, 50) * 0.1;

    return true;
}