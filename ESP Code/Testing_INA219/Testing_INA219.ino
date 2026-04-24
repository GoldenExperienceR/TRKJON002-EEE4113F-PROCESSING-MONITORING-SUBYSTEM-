#include <Wire.h>                 // I²C communication library
#include <Adafruit_INA219.h>     // High-level driver for INA219

// Create an INA219 object (this handles all register interactions internally)
Adafruit_INA219 ina219;


void setup(void) 
{
  Serial.begin(115200);          // Start serial communication for debugging

  // Some boards (e.g., Leonardo) need to wait for serial monitor to open
  while (!Serial) {
      delay(1);
  }
    
  Serial.println("Starting INA219 Test");
  
  // ==============================
  // INITIALIZE INA219 SENSOR
  // ==============================
  // This does multiple things internally:
  // - Starts I²C communication
  // - Checks if device responds at address (usually 0x40)
  // - Writes default configuration register
  // - Applies default calibration (32V, 2A range)
  if (!ina219.begin()) {
    Serial.println("Failed to find INA219 chip");

    // If the sensor is not detected, stop execution here
    // This usually means:
    // - Wiring issue
    // - Wrong I²C address
    // - Sensor not powered
    while (1) { delay(10); }
  }

  // ==============================
  // OPTIONAL CALIBRATION SETTINGS
  // ==============================
  // These change measurement range and precision.
  // Lower range = higher resolution (more precise readings)

  //ina219.setCalibration_32V_1A();   // Better precision for lower currents
  //ina219.setCalibration_16V_400mA(); // Even more precise, but smaller range

  // If you don’t call anything → default is:
  // setCalibration_32V_2A()

  Serial.println("Measuring voltage and current with INA219 ...");
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