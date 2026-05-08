/*
 * ============================================================
 * UART Packet Communication with Checksum Verification
 * ============================================================
 *
 * PURPOSE:
 * Demonstrates a robust UART communication protocol between two
 * devices. A structured 6-byte packet with checksum verification
 * ensures corrupted or false packets are detected and rejected.
 *
 * PACKET STRUCTURE (extensible for future sensor data):
 * [START_BYTE][MSG_TYPE][LENGTH][PAYLOAD x LENGTH][CHECKSUM][END_BYTE]
 *   0xAA        1 byte   1 byte    N bytes          1 byte    0x03
 *
 * CHECKSUM:
 * XOR of all payload bytes. Receiver recalculates and compares
 * to detect any corruption during transmission.
 *
 * HOW TO SWITCH BETWEEN PC TESTING AND ESP-TO-ESP:
 * Comment out the #define USE_ESP_TO_ESP line for PC testing via USB.
 * Uncomment it for ESP-to-ESP testing via GPIO 16 (RX) and GPIO 17 (TX).
 *  
 * Valid packet — LED ON  (should be ACCEPTED):
 *   AA 01 01 59 59 03
 *
 * Valid packet — LED OFF (should be ACCEPTED):
 *   AA 01 01 4E 4E 03
 *
 * Corrupted checksum (should be REJECTED — checksum mismatch):
 *   AA 01 01 59 FF 03
 *
 * Missing start byte (should be REJECTED — framing error):
 *   FF 01 01 59 59 03
 *
 * Missing end byte (should be REJECTED — framing error):
 *   AA 01 01 59 59 FF
 *
 * Partial packet — only 3 bytes (should TIMEOUT and be discarded):
 *   AA 01 01
 *
 * ============================================================
 */

// --- COMMUNICATION MODE SELECTION ---
// Comment out the line below for PC testing via USB serial monitor
// Uncomment it for ESP-to-ESP testing via GPIO 16 and GPIO 17
// #define USE_ESP_TO_ESP

#ifdef USE_ESP_TO_ESP
  // ESP-to-ESP mode — use hardware Serial2 on dedicated UART pins
  #define MY_SERIAL Serial2
  #define RX_PIN 16   // GPIO 16 — UART2 RX — receives data from comms subsystem
  #define TX_PIN 17   // GPIO 17 — UART2 TX — sends data to comms subsystem
#else
  // PC testing mode — use USB serial via CH340 chip
  #define MY_SERIAL Serial
#endif

// --- PIN DEFINITIONS ---
// GPIO 2 is used to toggle and external LED 
const int LED_PIN = 2;

// --- PACKET PROTOCOL DEFINITIONS ---
// These byte values define the structure of every packet
const byte START_BYTE = 0xAA;  // Marks the beginning of every valid packet
                                // 0xAA chosen as it is a recognisable alternating bit pattern
                                // that is unlikely to appear randomly in corrupted data
const byte END_BYTE   = 0x03;  // ETX — marks the end of every valid packet
                                // If this is missing the packet is truncated or corrupted

// --- MESSAGE TYPE DEFINITIONS ---
// MSG_TYPE byte tells the receiver what kind of data is in the payload
// This makes the protocol extensible — add new types as your design grows
const byte MSG_CMD    = 0x01;  // Command message — payload is a single command character
const byte MSG_DATA   = 0x02;  // Data message — payload is sensor data (future use)
const byte MSG_ACK    = 0x03;  // Acknowledgement — receiver confirms packet was accepted
const byte MSG_NACK   = 0x04;  // Negative acknowledgement — receiver reports rejection

// --- PROTOCOL TIMING ---
const unsigned long PACKET_TIMEOUT_MS = 50;  // Maximum time to wait for a complete packet
                                              // If all bytes do not arrive within 50ms
                                              // the partial packet is discarded
                                              // Prevents receiver locking up on incomplete transmissions

// --- STATISTICS TRACKING ---
// These counters provide evidence for acceptance testing validation
unsigned long packetsReceived = 0;   // Count of valid packets successfully processed
unsigned long packetsRejected = 0;   // Count of packets rejected due to any error
unsigned long checksumErrors  = 0;   // Count specifically of checksum failures
unsigned long framingErrors   = 0;   // Count specifically of framing errors
unsigned long timeoutErrors   = 0;   // Count specifically of timeout events

// ============================================================
// SETUP
// ============================================================
void setup() {

  #ifdef USE_ESP_TO_ESP
    // Initialise hardware Serial2 with explicit GPIO pin assignments
    // SERIAL_8N1 means 8 data bits, No parity, 1 stop bit — standard UART configuration
    MY_SERIAL.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
  #else
    // Initialise USB serial for PC testing
    MY_SERIAL.begin(115200);
  #endif

  // Short delay to allow serial port to stabilise before printing

  delay(100);

  // Configure LED pin as output and ensure it starts in the OFF state
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Print startup information including the protocol structure for reference
  MY_SERIAL.println("\n============================================");
  MY_SERIAL.println("  UART PACKET PROTOCOLTEST");
  MY_SERIAL.println("============================================");
  MY_SERIAL.println("Packet structure:");
  MY_SERIAL.println("  [0xAA][MSG_TYPE][LENGTH][PAYLOAD...][CHECKSUM][0x03]");
  MY_SERIAL.println("Checksum: XOR of all payload bytes");
  MY_SERIAL.println("--------------------------------------------");
  MY_SERIAL.println("READY — awaiting packets...\n");

// --- SELF-TEST MODE ---
  // Uncomment the block below to make the ESP32 send test packets to itself
  // This lets you validate the protocol without a hex-capable terminal
  
  /*
  delay(2000);  // Wait 2 seconds so you can see the startup messages first
  
  MY_SERIAL.println("\n=== STARTING SELF-TEST ===\n");
  
  // Test 1 — Valid packet LED ON
  MY_SERIAL.println("Test 1: Sending valid LED ON packet...");
  sendPacket('Y');
  delay(2000);
  
  // Test 2 — Valid packet LED OFF
  MY_SERIAL.println("Test 2: Sending valid LED OFF packet...");
  sendPacket('N');
  delay(2000);
  
  // Test 3 — Corrupted checksum (manually construct bad packet)
  MY_SERIAL.println("Test 3: Sending packet with corrupted checksum...");
  MY_SERIAL.write((byte)0xAA);  // START
  MY_SERIAL.write((byte)0x01);  // MSG_CMD
  MY_SERIAL.write((byte)0x01);  // LENGTH
  MY_SERIAL.write((byte)0x59);  // Payload 'Y'
  MY_SERIAL.write((byte)0xFF);  // WRONG checksum (should be 0x59)
  MY_SERIAL.write((byte)0x03);  // END
  delay(2000);
  
  // Test 4 — Missing start byte
  MY_SERIAL.println("Test 4: Sending packet with missing start byte...");
  MY_SERIAL.write((byte)0xFF);  // WRONG start byte
  MY_SERIAL.write((byte)0x01);  // MSG_CMD
  MY_SERIAL.write((byte)0x01);  // LENGTH
  MY_SERIAL.write((byte)0x59);  // Payload 'Y'
  MY_SERIAL.write((byte)0x59);  // Checksum
  MY_SERIAL.write((byte)0x03);  // END
  delay(2000);
  
  // Test 5 — Ten consecutive valid packets
  MY_SERIAL.println("Test 5: Sending 10 consecutive valid packets...");
  for (int i = 0; i < 10; i++) {
    sendPacket('Y');
    delay(2000);
  }
  
  MY_SERIAL.println("\n=== SELF-TEST COMPLETE ===");
  MY_SERIAL.println("Check statistics above for ATP evidence\n");
  */
}

// ============================================================
// MAIN LOOP
// ============================================================
void loop() {

  // Only attempt to read a packet if at least the minimum bytes are available
  // Minimum packet is 5 bytes: START + MSG_TYPE + LENGTH + CHECKSUM + END
  // (zero-length payload is technically valid for ACK/NACK messages)
  if (MY_SERIAL.available() >= 5) {

    // Step 1 — Check for valid start byte
    // Read the first available byte and check it is our expected start marker
    // If it is not 0xAA this is not a valid packet start — discard and return
    byte firstByte = MY_SERIAL.read();

    if (firstByte != START_BYTE) {
      // This byte is not a valid packet start
      // Could be noise, a partial previous packet, or misaligned data
      MY_SERIAL.println("REJECTED: No valid start byte — discarding");
      framingErrors++;
      printStatistics();
      return;  // Return to loop() — do not attempt to read rest of non-packet
    }

    // Step 2 — Read message type and payload length with timeout protection
    // We have confirmed the start byte — now wait for the rest of the packet
    // If it does not arrive within PACKET_TIMEOUT_MS we discard and move on
    unsigned long packetStartTime = millis();

    // Wait until we have enough bytes for MSG_TYPE and LENGTH fields
    while (MY_SERIAL.available() < 2) {
      if (millis() - packetStartTime > PACKET_TIMEOUT_MS) {
        MY_SERIAL.println("REJECTED: Timeout waiting for header bytes");
        timeoutErrors++;
        packetsRejected++;
        printStatistics();
        flushBuffer();  // Clear any remaining bytes from the incomplete packet
        return;
      }
    }

    byte msgType = MY_SERIAL.read();  // Read the message type byte
    byte length  = MY_SERIAL.read();  // Read the payload length byte

    // Step 3 — Wait for payload bytes plus checksum and end byte
    // Total remaining bytes needed = length (payload) + 1 (checksum) + 1 (end byte)
    while (MY_SERIAL.available() < length + 2) {
      if (millis() - packetStartTime > PACKET_TIMEOUT_MS) {
        MY_SERIAL.println("REJECTED: Timeout waiting for payload bytes");
        timeoutErrors++;
        packetsRejected++;
        printStatistics();
        flushBuffer();
        return;
      }
    }

    // Step 4 — Read payload bytes and calculate checksum simultaneously
    // Checksum is XOR of all payload bytes
    // We calculate it as we read rather than storing all bytes first
    byte calculatedChecksum = 0x00;   // Start with zero — XOR accumulator
    byte payload[32];                  // Buffer to store payload bytes — max 32 bytes
    
    // Clamp length to buffer size to prevent overflow
    if (length > 32) length = 32;

    for (int i = 0; i < length; i++) {
      payload[i] = MY_SERIAL.read();           // Read each payload byte
      calculatedChecksum ^= payload[i];         // XOR each byte into checksum accumulator
                                                // XOR of all payload bytes gives our expected checksum
    }

    // Step 5 — Read the received checksum and end byte
    byte receivedChecksum = MY_SERIAL.read();  // What the sender claims the checksum should be
    byte endByte          = MY_SERIAL.read();  // Should be 0x03 if packet is complete

    // Step 6 — Validate end byte (frame check)
    // If the end byte is wrong the packet is malformed or truncated
    if (endByte != END_BYTE) {
      MY_SERIAL.print("REJECTED: Framing error — expected 0x03, got 0x");
      MY_SERIAL.println(endByte, HEX);
      framingErrors++;
      packetsRejected++;
      printStatistics();
      return;
    }

    // Step 7 — Validate checksum (data integrity check)
    // If our calculated checksum does not match what the sender sent
    // the payload was corrupted during transmission
    if (calculatedChecksum != receivedChecksum) {
      MY_SERIAL.print("REJECTED: Checksum mismatch — calculated 0x");
      MY_SERIAL.print(calculatedChecksum, HEX);
      MY_SERIAL.print(", received 0x");
      MY_SERIAL.println(receivedChecksum, HEX);
      checksumErrors++;
      packetsRejected++;
      printStatistics();
      // Send NACK back to sender to inform them of the rejection
      sendNACK();
      return;
    }

    // Step 8 — Packet is valid — process it
    // All validation checks passed — this packet is authentic and uncorrupted
    packetsReceived++;
    MY_SERIAL.print("ACCEPTED: Valid packet received — MSG_TYPE=0x");
    MY_SERIAL.print(msgType, HEX);
    MY_SERIAL.print(" LENGTH=");
    MY_SERIAL.println(length);

    // Send ACK back to sender to confirm successful receipt
    sendACK();

    // Process the packet based on message type
    processPacket(msgType, payload, length);

    printStatistics();
  }



}

// ============================================================
// PACKET PROCESSING
// ============================================================
void processPacket(byte msgType, byte* payload, byte length) {

  if (msgType == MSG_CMD && length >= 1) {
    // Command message — first payload byte is the command character
    char cmd = toupper((char)payload[0]);

    if (cmd == 'Y') {
      digitalWrite(LED_PIN, HIGH);
      MY_SERIAL.println("COMMAND: LED turned ON");

    } else if (cmd == 'N') {
      digitalWrite(LED_PIN, LOW);
      MY_SERIAL.println("COMMAND: LED turned OFF");

    } else {
      MY_SERIAL.print("COMMAND: Unknown command received — 0x");
      MY_SERIAL.println(payload[0], HEX);
    }

  } else if (msgType == MSG_DATA) {
    // Data message — placeholder for future sensor data packets
    // When you extend the protocol to send real sensor readings
    // handle the multi-byte payload parsing here
    MY_SERIAL.println("DATA: Sensor data packet received (handler not yet implemented)");

  } else {
    MY_SERIAL.print("WARNING: Unrecognised message type — 0x");
    MY_SERIAL.println(msgType, HEX);
  }
}

// ============================================================
// SEND ACK — positive acknowledgement
// Tells the sender their packet was received and verified successfully
// ============================================================
void sendACK() {
  byte ackPayload = 0x06;          // ACK control character
  byte checksum   = ackPayload;    // Checksum of single byte payload is just that byte
  MY_SERIAL.write(START_BYTE);
  MY_SERIAL.write(MSG_ACK);
  MY_SERIAL.write((byte)1);        // Length = 1 byte payload
  MY_SERIAL.write(ackPayload);
  MY_SERIAL.write(checksum);
  MY_SERIAL.write(END_BYTE);
}

// ============================================================
// SEND NACK — negative acknowledgement
// Tells the sender their packet was rejected due to checksum failure
// ============================================================
void sendNACK() {
  byte nackPayload = 0x15;         // NACK control character
  byte checksum    = nackPayload;
  MY_SERIAL.write(START_BYTE);
  MY_SERIAL.write(MSG_NACK);
  MY_SERIAL.write((byte)1);        // Length = 1 byte payload
  MY_SERIAL.write(nackPayload);
  MY_SERIAL.write(checksum);
  MY_SERIAL.write(END_BYTE);
}

// ============================================================
// SEND PACKET — use this on the SENDER ESP32
// Constructs and transmits a valid command packet
// Example: sendPacket('Y') turns the LED on the receiver on
// ============================================================
void sendPacket(char cmd) {
  byte payload  = (byte)cmd;
  byte checksum = payload;         // XOR of single payload byte is just that byte

  MY_SERIAL.write(START_BYTE);     // 1. Start marker
  MY_SERIAL.write(MSG_CMD);        // 2. Message type — command
  MY_SERIAL.write((byte)1);        // 3. Payload length — 1 byte
  MY_SERIAL.write(payload);        // 4. The command character
  MY_SERIAL.write(checksum);       // 5. Checksum — XOR of payload
  MY_SERIAL.write(END_BYTE);       // 6. End marker
}

// ============================================================
// FLUSH BUFFER — discard all pending bytes in the receive buffer
// Called after timeout or framing errors to clear corrupt data
// ============================================================
void flushBuffer() {
  while (MY_SERIAL.available()) {
    MY_SERIAL.read();  // Read and discard each byte until buffer is empty
  }
}

// ============================================================
// PRINT STATISTICS 
// Prints running totals of accepted and rejected packets
// ============================================================
void printStatistics() {
  MY_SERIAL.println("--------------------------------------------");
  MY_SERIAL.print("  Packets accepted:  "); MY_SERIAL.println(packetsReceived);
  MY_SERIAL.print("  Packets rejected:  "); MY_SERIAL.println(packetsRejected);
  MY_SERIAL.print("  Checksum errors:   "); MY_SERIAL.println(checksumErrors);
  MY_SERIAL.print("  Framing errors:    "); MY_SERIAL.println(framingErrors);
  MY_SERIAL.print("  Timeout events:    "); MY_SERIAL.println(timeoutErrors);
  MY_SERIAL.println("--------------------------------------------\n");
}