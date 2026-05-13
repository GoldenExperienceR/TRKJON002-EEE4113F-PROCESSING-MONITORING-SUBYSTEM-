#include "SHTC3_Driver.h"

#include <Wire.h>

// =====================================================
// SHTC3 Definitions
// =====================================================

#define SHTC3_ADDRESS             0x70

#define SHTC3_CMD_WAKE            0x3517
#define SHTC3_CMD_SLEEP           0xB098
#define SHTC3_CMD_MEASURE_NORMAL  0x7866

// =====================================================
// Private Function Prototypes
// =====================================================

static bool SHTC3_WriteCommand(uint16_t command);

static bool SHTC3_ReadBytes(uint8_t* buffer, uint8_t length);

// =====================================================
// Public Functions
// =====================================================

bool SHTC3_Init(void)
{
    Wire.begin(21, 22);

    delay(10);

    return SHTC3_Wake();
}

bool SHTC3_ReadData(float* temperature, float* humidity)
{
    uint8_t rxBuffer[6];

    // Wake sensor before measurement
    if (!SHTC3_Wake())
    {
        return false;
    }

    delay(1);

    // Start measurement
    if (!SHTC3_WriteCommand(SHTC3_CMD_MEASURE_NORMAL))
    {
        return false;
    }

    // Wait for conversion
    delay(20);

    // Read returned bytes
    if (!SHTC3_ReadBytes(rxBuffer, 6))
    {
        return false;
    }

    // Extract raw temperature
    uint16_t rawTemperature =
        ((uint16_t)rxBuffer[0] << 8) |
        rxBuffer[1];

    // Extract raw humidity
    uint16_t rawHumidity =
        ((uint16_t)rxBuffer[3] << 8) |
        rxBuffer[4];

    // Convert temperature
    *temperature =
        -45.0f +
        175.0f *
        ((float)rawTemperature / 65535.0f);

    // Convert humidity
    *humidity =
        100.0f *
        ((float)rawHumidity / 65535.0f);

    // Return sensor to sleep mode
    SHTC3_Sleep();

    return true;
}

bool SHTC3_Wake(void)
{
    bool status = SHTC3_WriteCommand(SHTC3_CMD_WAKE);

    delay(1);

    return status;
}

bool SHTC3_Sleep(void)
{
    return SHTC3_WriteCommand(SHTC3_CMD_SLEEP);
}

// =====================================================
// Private Functions
// =====================================================

static bool SHTC3_WriteCommand(uint16_t command)
{
    Wire.beginTransmission(SHTC3_ADDRESS);

    Wire.write(command >> 8);
    Wire.write(command & 0xFF);

    return (Wire.endTransmission() == 0);
}

static bool SHTC3_ReadBytes(uint8_t* buffer, uint8_t length)
{
    Wire.requestFrom(SHTC3_ADDRESS, length);

    if (Wire.available() != length)
    {
        return false;
    }

    for (uint8_t i = 0; i < length; i++)
    {
        buffer[i] = Wire.read();
    }

    return true;
}