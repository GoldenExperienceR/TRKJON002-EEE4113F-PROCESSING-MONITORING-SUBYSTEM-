#include "ina219_driver.h"

#include <Wire.h>

#include <Adafruit_INA219.h>

#define INA219_ADDRESS 0x40

static Adafruit_INA219 ina219(INA219_ADDRESS);

static bool ina219Healthy = false;

/*====================================
  INITIALIZATION
====================================*/

bool INA219_Init(void)
{
    /*
      Explicit ESP32 I2C setup
    */

    Wire.begin(21, 22);

    delay(1000);

    /*
      Initialize INA219
    */

    if(!ina219.begin(&Wire))
    {
        Serial.println("INA219 Initialization Failed");

        ina219Healthy = false;

        return false;
    }

    Serial.println("INA219 Initialized");

    ina219Healthy = true;

    return true;
}

/*====================================
  READ POWER DATA
====================================*/

bool INA219_ReadData(
    float* busVoltage,
    float* shuntVoltage,
    float* loadVoltage,
    float* current,
    float* power
)
{
    if(!ina219Healthy)
    {
        return false;
    }

    /*
      Read sensor values
    */

    *shuntVoltage = ina219.getShuntVoltage_mV();

    *busVoltage = ina219.getBusVoltage_V();

    *current = ina219.getCurrent_mA();

    *power = ina219.getPower_mW();

    /*
      Corrected load voltage
    */

    *loadVoltage = *busVoltage + (*shuntVoltage / 1000.0);

    return true;
}