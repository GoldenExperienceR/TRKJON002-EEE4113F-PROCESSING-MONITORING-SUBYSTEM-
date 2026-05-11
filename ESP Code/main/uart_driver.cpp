#include "uart_driver.h"

#include "firmware_config.h"

#include <Arduino.h>

// =====================================================
// UART MODE SELECTION
// =====================================================

#ifdef USE_USB_UART

    // =================================================
    // USB SERIAL MODE
    // =================================================

    #define CommsUART Serial

#else

    // =================================================
    // UART2 GPIO MODE
    // =================================================

    HardwareSerial CommsUART(2);

#endif

// =====================================================
// INITIALIZATION
// =====================================================

void UART_HAL_Init(void)
{
#ifdef USE_USB_UART

    CommsUART.begin(UART_BAUD_RATE);

#else

    CommsUART.begin(
        UART_BAUD_RATE,
        SERIAL_8N1,
        UART2_RX_PIN,
        UART2_TX_PIN
    );

#endif
}

// =====================================================
// TRANSMIT FUNCTIONS
// =====================================================

void UART_HAL_SendByte(uint8_t byteData)
{
    CommsUART.write(byteData);
}

void UART_HAL_SendBuffer(
    const uint8_t* buffer,
    uint16_t length)
{
    CommsUART.write(buffer, length);
}

// =====================================================
// RECEIVE FUNCTIONS
// =====================================================

uint16_t UART_HAL_BytesAvailable(void)
{
    return CommsUART.available();
}

uint8_t UART_HAL_ReadByte(void)
{
    return CommsUART.read();
}

// =====================================================
// BUFFER FLUSH
// =====================================================

void UART_HAL_Flush(void)
{
    while(CommsUART.available())
    {
        CommsUART.read();
    }
}
