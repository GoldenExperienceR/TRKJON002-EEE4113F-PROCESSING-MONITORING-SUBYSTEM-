#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

void setup()
{
    Serial.begin(115200);

    Serial.println("Starting INA219 Test");

    // Explicit ESP32 I2C setup
    Wire.begin(21, 22);

    // Allow bus and sensor to stabilize
    delay(1000);

    // Force library to use configured Wire bus
    if (!ina219.begin(&Wire))
    {
        Serial.println("Failed to find INA219 chip");

        while (1)
        {
            delay(10);
        }
    }

    Serial.println("INA219 Found!");
}


void loop(void) 
{
  // ==============================
  // VARIABLE DECLARATIONS
  // ==============================
  float shuntvoltage = 0;   // Voltage across shunt resistor (mV)
  float busvoltage = 0;     // Voltage at load side (V)
  float current_mA = 0;     // Current through load (mA)
  float loadvoltage = 0;    // Actual voltage across load (V)
  float power_mW = 0;       // Power consumption (mW)
  float power_load = 0;      // Load consumption (mW)


  // ==============================
  // SENSOR READINGS
  // ==============================

  // Measures voltage across internal shunt resistor
  // This is how current is calculated internally (Ohm’s Law)
  shuntvoltage = ina219.getShuntVoltage_mV();

  // Measures voltage on the load side (after the shunt resistor)
  busvoltage = ina219.getBusVoltage_V();

  // Returns current (already calculated using calibration + shunt voltage)
  current_mA = ina219.getCurrent_mA();

  // Returns power (internally computed as V * I)
  power_mW = ina219.getPower_mW();
  


  // ==============================
  // LOAD VOLTAGE CALCULATION
  // ==============================
  // Bus voltage is measured AFTER the shunt resistor.
  // So to get actual load voltage, we add the drop across the shunt.
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  power_load = current_mA*loadvoltage ;

  // ==============================
  // PRINT RESULTS
  // ==============================
  Serial.print("Bus Voltage:   ");
  Serial.print(busvoltage);
  Serial.println(" V");

  Serial.print("Shunt Voltage: ");
  Serial.print(shuntvoltage);
  Serial.println(" mV");

  Serial.print("Load Voltage:  ");
  Serial.print(loadvoltage);
  Serial.println(" V");

  Serial.print("Current:       ");
  Serial.print(current_mA);
  Serial.println(" mA");

   Serial.print("Power Interal :         ");
  Serial.print(power_mW);
  Serial.println(" mW");


  Serial.print("Load Power Manual :         ");
  Serial.print(power_load);
  Serial.println(" mW");

  Serial.println("");

  delay(3000);  // Wait 2 seconds between readings
}