#include <SPI.h>   // Include SPI library which handles the low-level SPI communication protocol
#include <SD.h>    // Include SD library which handles file system operations on the SD card

#define CS_PIN 5   // GPIO 5 is your chip select pin — tells the SD card when the ESP32 wants to talk to it

void setup() {
  Serial.begin(115200);  // Start serial communication at 115200 baud
  delay(1000);           // Wait 1 second for serial monitor to connect before printing anything

  Serial.println("MicroSD card test starting...");
  Serial.println("-------------------------------");

  // Attempt to initialise the SD card
  // SD.begin() sends SPI commands to the card and checks for a valid FAT32 file system
  // It returns true if successful and false if it cannot communicate with the card
  if (!SD.begin(CS_PIN)) {
    Serial.println("ERROR: SD card initialisation failed");
    Serial.println("Check: CS pin is GPIO 5, VCC is 5V, card is FAT32 formatted");
    while (1);  // Halt program — no point continuing without SD card
  }

  Serial.println("SD card initialised successfully");
  Serial.println("-------------------------------");

  // Print the card type for diagnostic information
  // This confirms what type of card the library detected
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card inserted");
    while (1);
  }
  Serial.print("Card type: ");
  if (cardType == CARD_MMC)       Serial.println("MMC");
  else if (cardType == CARD_SD)   Serial.println("SDSC");
  else if (cardType == CARD_SDHC) Serial.println("SDHC");  // Your Netac 16GB should show this
  else                            Serial.println("Unknown");

  // Print the total card size in megabytes
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.print("Card size: ");
  Serial.print((uint64_t)cardSize);
  Serial.println(" MB");
  Serial.println("-------------------------------");

  // WRITE TEST
  // Attempt to create and write to a test file called testlog.csv
  // FILE_WRITE opens the file for writing — creates it if it does not exist
  // FILE_WRITE also appends to the file if it already exists
  Serial.println("Writing 10 test entries to testlog.csv...");

  File logFile = SD.open("/testlog.csv", FILE_WRITE);  // Open file for writing

  if (!logFile) {
    // If the file could not be opened something is wrong with the card or file system
    Serial.println("ERROR: Could not open testlog.csv for writing");
    while (1);
  }

  // Write a header row first so the CSV is readable in a spreadsheet
  logFile.println("entry_number,timestamp_ms,test_value,message");

  // Write 10 test data rows
  for (int i = 1; i <= 10; i++) {
    logFile.print(i);                    // Entry number
    logFile.print(",");
    logFile.print(millis());             // Timestamp in milliseconds since boot
    logFile.print(",");
    logFile.print(i * 3.14, 2);         // A test value — i multiplied by pi to 2 decimal places
    logFile.print(",");
    logFile.print("test_entry_");
    logFile.println(i);                  // Message field with entry number

    Serial.print("Written entry ");
    Serial.println(i);                   // Confirm each write in serial monitor
  }

  logFile.close();  // CRITICAL — always close the file after writing
                    // Closing flushes the write buffer to the card and updates the file system
                    // If you do not close the file the data may not actually be saved

  Serial.println("Write complete — file closed");
  Serial.println("-------------------------------");

  // READ BACK TEST
  // Re-open the file in read mode and print contents to serial monitor
  // This confirms what was written is actually stored correctly on the card
  Serial.println("Reading back testlog.csv contents...");
  Serial.println("-------------------------------");

  File readFile = SD.open("/testlog.csv", FILE_READ);  // Open file for reading

  if (!readFile) {
    Serial.println("ERROR: Could not open testlog.csv for reading");
    while (1);
  }

  // Read the file character by character and print to serial monitor
  // available() returns the number of bytes remaining to be read
  while (readFile.available()) {
    Serial.write(readFile.read());  // Read one byte and write it to serial monitor
  }

  readFile.close();  // Close the file after reading

  Serial.println("-------------------------------");
  Serial.println("Read back complete");
  Serial.println("-------------------------------");
  Serial.println("Now remove card and check testlog.csv on your PC");
  Serial.println("All tests complete — ATP-011 evidence ready");
}

void loop() {
  // Nothing runs in loop for this test
  // All SD card operations are performed once in setup()
  // This prevents repeatedly writing to the card every loop iteration
}