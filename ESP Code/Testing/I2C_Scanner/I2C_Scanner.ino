#include <Wire.h>  // Include the Wire library which handles I2C communication on the ESP32

void setup() {
  Serial.begin(115200);  // Start serial communication at 115200 baud so we can print to the serial monitor

  Wire.begin(21, 22);  // Initialise the I2C bus — SDA on GPIO 21, SCL on GPIO 22 (ESP32 defaults)
  
  delay(1000);  // Wait 1 second after startup to allow all I2C devices to power up and stabilise
  
  Serial.println("I2C scanner starting...");  
}

void loop() {
  int deviceCount = 0;  // Counter to track how many devices respond during this scan

  // Loop through every possible 7-bit I2C address from 1 to 126
  // Address 0 is reserved so we start at 1
  for (byte address = 1; address < 127; address++) {
    
    Wire.beginTransmission(address);  // Send the address onto the I2C bus — like knocking on a door
    byte error = Wire.endTransmission();  // End the transmission and capture the response
    // error = 0 means the device acknowledged — it answered the knock
    // error = 2 means no device responded at this address
    // error = 4 means an unknown error occurred on the bus

    if (error == 0) {
      // A device responded at this address — print it to serial monitor
      Serial.print("Device found at address 0x");
      if (address < 16) Serial.print("0");  // Print leading zero for addresses below 0x10 for neat formatting
      Serial.println(address, HEX);  // Print the address in hexadecimal format
      deviceCount++;  // Increment our device counter
    }
  }

  // After scanning all 127 addresses report the result
  if (deviceCount == 0) {
    Serial.println("No I2C devices found");  // Nothing responded — likely a wiring or power issue
  } else {
    Serial.print("Scan complete — ");
    Serial.print(deviceCount);
    Serial.println(" device(s) found");  // Report total number of responding devices
  }

  Serial.println("---");  // Print a divider line between each scan cycle for readability
  
  delay(3000);  // Wait 3 seconds then scan again — the loop repeats every 3 seconds
}