#include <Wire.h>      // Include the Wire library which handles I2C communication on the ESP32
#include "RTClib.h"    // Include the RTClib library which handles all DS3231 register communication for us

RTC_DS3231 rtc;        // Create an RTC object called 'rtc' — this is our software handle for the DS3231 module

void setup() {
  Serial.begin(9600);  // Start serial communication at 9600 baud — note this must match your serial monitor baud rate setting
  Wire.begin();        // Initialise the I2C bus using default pins — on ESP32 this is SDA=GPIO21 and SCL=GPIO22

  // Attempt to initialise communication with the DS3231 module
  // rtc.begin() sends an I2C transaction to address 0x68 and checks for a response
  // If the module is not connected, not powered, or wired incorrectly this will return false
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");  // Print error message so we know the module was not detected
    while (1);                            // Halt the program permanently — no point continuing without the RTC
  }

  // This line sets the DS3231 time to match your computer's compile time
  // F(__DATE__) gives the current date at the moment you clicked upload e.g. "Apr 15 2026"
  // F(__TIME__) gives the current time at the moment you clicked upload e.g. "14:32:05"
  // It is commented out because you only need to run it once to set the time
  // After setting the time comment it out again and re-upload — otherwise it resets to compile time every restart
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  DateTime now = rtc.now();  // Ask the DS3231 for the current date and time — returns a DateTime object

  // Print the date in YYYY/MM/DD format
  // DEC means print the number in decimal (base 10) format rather than hex or binary
  Serial.print(now.year(), DEC);    // Print the year e.g. 2026
  Serial.print('/');                 // Print separator
  Serial.print(now.month(), DEC);   // Print the month e.g. 4 for April
  Serial.print('/');                 // Print separator
  Serial.print(now.day(), DEC);     // Print the day of the month e.g. 15
  Serial.print(' ');                 // Print a space between date and time

  // Print the time in HH:MM:SS format
  Serial.print(now.hour(), DEC);    // Print the hour in 24-hour format e.g. 14
  Serial.print(':');                 // Print separator
  Serial.print(now.minute(), DEC);  // Print the minutes e.g. 32
  Serial.print(':');                 // Print separator
  Serial.print(now.second(), DEC);  // Print the seconds e.g. 05
  Serial.println();                  // Print a newline character so each reading appears on its own line

  delay(1000);  // Wait 1 second before reading and printing the time again
}