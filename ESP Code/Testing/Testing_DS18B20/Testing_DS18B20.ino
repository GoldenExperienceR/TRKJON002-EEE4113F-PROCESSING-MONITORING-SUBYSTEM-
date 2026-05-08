#include <OneWire.h>          // Handles the low-level 1-Wire communication protocol
#include <DallasTemperature.h> // Sits on top of OneWire and handles DS18B20 specific commands

#define ONE_WIRE_BUS 25       // GPIO 25 is your 1-Wire DATA line — where centre pin of DS18B20 connects

OneWire oneWire(ONE_WIRE_BUS);        // Create a OneWire object on GPIO 25
DallasTemperature sensors(&oneWire);  // Pass the OneWire object to the DallasTemperature library

void setup() {
  Serial.begin(115200);        // Start serial communication at 115200 baud
  sensors.begin();             // Initialise the DallasTemperature library — scans the 1-Wire bus for devices

  Serial.println("DS18B20 temperature sensor starting...");

  // Print how many DS18B20 sensors were found on the bus
  // Should print 1 if your single sensor is correctly wired
  Serial.print("Devices found on 1-Wire bus: ");
  Serial.println(sensors.getDeviceCount());
}

void loop() {
  sensors.requestTemperatures();  // Send command to ALL DS18B20 sensors on the bus to take a temperature reading
                                  // This triggers the internal conversion — takes up to 750ms at full resolution

  float tempC = sensors.getTempCByIndex(0);  // Get the temperature in Celsius from the first sensor (index 0)
                                              // Index 0 means the first device found on the bus

  // Check if the reading is valid
  // The library returns -127 if it cannot communicate with the sensor
  if (tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: could not read temperature — check wiring and pull-up resistor");
  } else {
    Serial.print("Temperature: ");
    Serial.print(tempC);          // Print temperature in Celsius
    Serial.print(" C  |  ");
    Serial.print(tempC * 9.0 / 5.0 + 32.0);  // Convert and print in Fahrenheit as well
    Serial.println(" F");
  }

  delay(1000);  // Wait 1 second before next reading
}