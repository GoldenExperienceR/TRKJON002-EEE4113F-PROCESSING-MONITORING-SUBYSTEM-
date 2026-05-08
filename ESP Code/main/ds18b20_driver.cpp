#include "ds18b20_driver.h"

#include <Arduino.h>

#include <OneWire.h>

#include <DallasTemperature.h>

/*====================================
  CONFIGURATION
====================================*/

#define ONE_WIRE_BUS 25

/*====================================
  PRIVATE OBJECTS
====================================*/

static OneWire oneWire(ONE_WIRE_BUS);

static DallasTemperature sensors(&oneWire);

static bool ds18b20Healthy = false;

/*====================================
  INITIALIZATION
====================================*/

bool DS18B20_Init(void)
{
    sensors.begin();

    Serial.println("DS18B20 Initializing...");

    /*
      Verify at least one sensor exists
    */

    if(sensors.getDeviceCount() < 1)
    {
        Serial.println("No DS18B20 Sensors Found");

        ds18b20Healthy = false;

        return false;
    }

    Serial.print("DS18B20 Devices Found: ");

    Serial.println(sensors.getDeviceCount());

    ds18b20Healthy = true;

    return true;
}

/*====================================
  READ TEMPERATURE
====================================*/

bool DS18B20_ReadTemperature(
    float* temperature
)
{
    if(!ds18b20Healthy)
    {
        return false;
    }

    /*
      Request temperature conversion
    */

    sensors.requestTemperatures();

    /*
      Read first sensor on bus
    */

    float tempC = sensors.getTempCByIndex(0);

    /*
      Validate reading
    */

    if(tempC == DEVICE_DISCONNECTED_C)
    {
        ds18b20Healthy = false;

        return false;
    }

    *temperature = tempC;

    return true;
}