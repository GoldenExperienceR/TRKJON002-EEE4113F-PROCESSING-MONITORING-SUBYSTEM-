#include "ntc_driver.h"

#include <Arduino.h>

#include <math.h>

/*====================================
  PIN CONFIGURATION
====================================*/

#define NTC_PIN_1 33

#define NTC_PIN_2 32

/*====================================
  THERMISTOR CONSTANTS
====================================*/

#define NOMINAL_RESISTANCE  100000.0

#define NOMINAL_TEMPERATURE 25.0

#define B_COEFFICIENT       3950.0

#define REFERENCE_RESISTOR  100000.0

/*====================================
  ADC CONFIGURATION
====================================*/

#define ADC_RESOLUTION 4095.0

#define SUPPLY_VOLTAGE 3.3

/*====================================
  HEALTH FLAGS
====================================*/

static bool ntc1Healthy = false;

static bool ntc2Healthy = false;

/*====================================
  PRIVATE TEMPERATURE FUNCTION
====================================*/

static float calculateTemperature(int pin)
{
    /*
      Read ADC value
    */

    int rawADC = analogRead(pin);

    /*
      Prevent divide-by-zero
    */

    if(rawADC <= 0)
    {
        return NAN;
    }

    /*
      Convert ADC reading to voltage
    */

    float voltage =
        (rawADC / ADC_RESOLUTION) * SUPPLY_VOLTAGE;

    /*
      Prevent invalid voltage calculations
    */

    if(voltage <= 0.0)
    {
        return NAN;
    }

    /*
      Calculate thermistor resistance
    */

    float resistance =
        REFERENCE_RESISTOR *
        (SUPPLY_VOLTAGE - voltage) /
        voltage;

    /*
      Steinhart-Hart B parameter equation
    */

    float steinhart =
        resistance / NOMINAL_RESISTANCE;

    steinhart = log(steinhart);

    steinhart /= B_COEFFICIENT;

    steinhart +=
        1.0 / (NOMINAL_TEMPERATURE + 273.15);

    steinhart = 1.0 / steinhart;

    float temperatureC =
        steinhart - 273.15;

    return temperatureC;
}

/*====================================
  INTIALISING FUNCTION
====================================*/
bool NTC_Init(void)
{
    pinMode(NTC_PIN_1, INPUT);

    pinMode(NTC_PIN_2, INPUT);

    Serial.println("NTC Driver Initialized");

    ntc1Healthy = true;

    ntc2Healthy = true;

    return true;
}
/*====================================
  READ FUNCTIONs
====================================*/
bool NTC_ReadTemperature1(
    float* temperature
)
{
    float temp =
        calculateTemperature(NTC_PIN_1);

    if(isnan(temp))
    {
        ntc1Healthy = false;

        return false;
    }

    *temperature = temp;

    ntc1Healthy = true;

    return true;
}

bool NTC_ReadTemperature2(
    float* temperature
)
{
    float temp =
        calculateTemperature(NTC_PIN_2);

    if(isnan(temp))
    {
        ntc2Healthy = false;

        return false;
    }

    *temperature = temp;

    ntc2Healthy = true;

    return true;
}