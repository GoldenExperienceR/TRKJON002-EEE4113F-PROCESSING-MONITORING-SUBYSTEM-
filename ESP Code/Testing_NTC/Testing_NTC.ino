#include <math.h>  // Include math library for the log() function used in the Steinhart-Hart equation

// Define which GPIO pins your thermistors are connected to
#define NTC_PIN_1 33  // NTC thermistor 1 midpoint connects to GPIO 33
#define NTC_PIN_2 32  // NTC thermistor 2 midpoint connects to GPIO 32

// Define the thermistor and circuit constants
#define NOMINAL_RESISTANCE  100000  // Resistance of thermistor at nominal temperature (100kΩ at 25°C)
#define NOMINAL_TEMPERATURE 25      // Nominal temperature in Celsius (25°C)
#define B_COEFFICIENT       3950    // B value of your specific thermistor — from component specification
#define REFERENCE_RESISTOR  100000  // Value of your fixed reference resistor in the voltage divider (100kΩ)

// Define ADC properties
#define ADC_RESOLUTION 4095   // ESP32 ADC is 12-bit — maximum raw reading is 4095
#define SUPPLY_VOLTAGE 3.3    // Supply voltage on the 3.3V rail

void setup() {
  Serial.begin(115200);  // Start serial communication at 115200 baud
  
  // Set both ADC pins as inputs
  pinMode(NTC_PIN_1, INPUT);  // GPIO 33 as input for thermistor 1
  pinMode(NTC_PIN_2, INPUT);  // GPIO 32 as input for thermistor 2
  
  Serial.println("NTC thermistor test starting...");
  Serial.println("-------------------------------");
    Serial.println("Configuration: NTC on top (3.3V), reference resistor on bottom (GND)");
  Serial.println("-------------------------------");
}

float readTemperature(int pin) {
  // Step 1 — read the raw ADC value from the pin
  // analogRead returns a value between 0 and 4095
  // 0 = 0V at the pin, 4095 = 3.3V at the pin
  int rawADC = analogRead(pin);
  
  // Step 2 — convert raw ADC reading to voltage
  // voltage = (rawADC / maxADC) x supplyVoltage
  float voltage = (rawADC / (float)ADC_RESOLUTION) * SUPPLY_VOLTAGE;
  
  // Step 3 — calculate thermistor resistance from voltage divider formula
  // The thermistor is the top resistor and reference is the bottom resistor
  // R_ntc = R_ref x (Vsupply - V)/ V
  float resistance = REFERENCE_RESISTOR * (SUPPLY_VOLTAGE - voltage) / voltage ;
  
  // Step 4 — convert resistance to temperature using Steinhart-Hart B parameter equation
  // 1/T = 1/T_nominal + (1/B) x ln(R/R_nominal)
  // where temperatures are in Kelvin
  float steinhart = resistance / NOMINAL_RESISTANCE;       // R/R_nominal
  steinhart = log(steinhart);                               // ln(R/R_nominal)
  steinhart /= B_COEFFICIENT;                              // divide by B coefficient
  steinhart += 1.0 / (NOMINAL_TEMPERATURE + 273.15);      // add 1/T_nominal (converted to Kelvin)
  steinhart = 1.0 / steinhart;                             // invert to get temperature in Kelvin
  float temperatureC = steinhart - 273.15;                 // convert Kelvin to Celsius
  
  return temperatureC;  // Return the calculated temperature in Celsius
}

void loop() {
  // Read raw ADC values for diagnostic purposes
  int raw1 = analogRead(NTC_PIN_1);  // Raw ADC reading from thermistor 1
  int raw2 = analogRead(NTC_PIN_2);  // Raw ADC reading from thermistor 2
  
  // Calculate temperatures for both thermistors
  float temp1 = readTemperature(NTC_PIN_1);  // Temperature from thermistor 1 in Celsius
  float temp2 = readTemperature(NTC_PIN_2);  // Temperature from thermistor 2 in Celsius
  
  // Print thermistor 1 results
  Serial.println("=== NTC Thermistor Readings ===");
  Serial.print("NTC 1 — Raw ADC: ");
  Serial.print(raw1);                // Print raw ADC value for diagnostics
  Serial.print("  |  Temperature: ");
  Serial.print(temp1, 1);            // Print temperature to 1 decimal place
  Serial.println(" C");

  // Print thermistor 2 results  
  Serial.print("NTC 2 — Raw ADC: ");
  Serial.print(raw2);                // Print raw ADC value for diagnostics
  Serial.print("  |  Temperature: ");
  Serial.print(temp2, 1);            // Print temperature to 1 decimal place
  Serial.println(" C");
  
  // Print the difference between the two thermistors
  // Under stable conditions this should be less than 3 degrees per SP-02
  float difference = abs(temp1 - temp2);
  Serial.print("Difference between sensors: ");
  Serial.print(difference, 1);
  Serial.println(" C");
  
  // Flag if sensors disagree beyond specification
  if (difference > 3.0) {
    Serial.println("WARNING — sensors disagree beyond 3 degrees C threshold");
  } else {
    Serial.println("Sensors agree within specification");
  }
  
  Serial.println("-------------------------------");
  delay(2000);  // Wait 2 seconds between readings
}