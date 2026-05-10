#include "uart_driver.h"

#define UART_BAUDRATE 115200

// =====================================================
// UART MODE SELECTION 
// =====================================================

// Uncomment for USB Serial debugging
#define USE_USB_UART

#ifdef USE_USB_UART

    // =================================================
    // USB SERIAL MODE
    // =================================================

    #define CommsUART Serial

#else

    // =================================================
    // UART2 GPIO MODE
    // =================================================

    #define UART_RX_PIN 16
    #define UART_TX_PIN 17

    HardwareSerial CommsUART(2);

#endif

// =====================================================
// INITIALISATION
// =====================================================

void UART_HAL_Init(void)
{
#ifdef USE_USB_UART

    CommsUART.begin(UART_BAUDRATE);

#else

    CommsUART.begin(
        UART_BAUDRATE,
        SERIAL_8N1,
        UART_RX_PIN,
        UART_TX_PIN);

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
    while (CommsUART.available())
    {
        CommsUART.read();
    }
}