#include "uart_driver.h"

#define UART_BAUDRATE 115200
#define UART_RX_PIN   16
#define UART_TX_PIN   17

HardwareSerial CommsUART(2);

void UART_HAL_Init(void)
{
    CommsUART.begin(UART_BAUDRATE, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);
}

void UART_HAL_SendByte(uint8_t byteData)
{
    CommsUART.write(byteData);
}

void UART_HAL_SendBuffer(uint8_t* buffer, uint16_t length)
{
    CommsUART.write(buffer, length);
}

bool UART_HAL_ByteAvailable(void)
{
    return CommsUART.available();
}

uint8_t UART_HAL_ReadByte(void)
{
    return CommsUART.read();
}

void UART_HAL_Flush(void)
{
    while (CommsUART.available())
    {
        CommsUART.read();
    }
}