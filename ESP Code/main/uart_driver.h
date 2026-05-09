#ifndef UART_HAL_H
#define UART_HAL_H

#include <Arduino.h>

void UART_HAL_Init(void);

void UART_HAL_SendByte(uint8_t byteData);

void UART_HAL_SendBuffer(uint8_t* buffer, uint16_t length);

bool UART_HAL_ByteAvailable(void);

uint8_t UART_HAL_ReadByte(void);

void UART_HAL_Flush(void);

#endif