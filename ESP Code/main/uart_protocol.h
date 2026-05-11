#ifndef UART_PROTOCOL_H
#define UART_PROTOCOL_H

#include <Arduino.h>
#include "telemetry_types.h"

#define START_BYTE 0xAA
#define END_BYTE   0x03

#define MSG_ACK        0x03
#define MSG_NACK       0x04
#define MSG_TELEMETRY  0x10

#define MAX_PAYLOAD_SIZE 128
// Estimated Packet Size: 
// 2 × uint32_t = 8 bytes
// 13 × float = 52 bytes

typedef struct
{
    uint8_t msgType;
    uint8_t payloadLength;
    uint8_t payload[MAX_PAYLOAD_SIZE];

} UARTPacket_t;

typedef enum
{
    UART_RESPONSE_NONE = 0,
    UART_RESPONSE_ACK,
    UART_RESPONSE_NACK,
    UART_RESPONSE_TIMEOUT,
    UART_RESPONSE_INVALID

} UARTResponse_t;

void UART_PROTOCOL_Init(void);

uint8_t UART_PROTOCOL_CalculateChecksum(const uint8_t* payload,
                                        uint8_t length);

void UART_PROTOCOL_SendPacket(uint8_t msgType,
                              const uint8_t* payload,
                              uint8_t length);

bool UART_PROTOCOL_ReadPacket(UARTPacket_t* packet);

void UART_PROTOCOL_SendACK(void);

void UART_PROTOCOL_SendNACK(void);

void UART_PROTOCOL_SendTelemetry(
    TelemetryPacket_t* telemetryData);

bool UART_PROTOCOL_WaitForACK(void);

UARTResponse_t UART_PROTOCOL_WaitForResponse(
    uint32_t timeout_ms);

#endif