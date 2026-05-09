#ifndef UART_PROTOCOL_H
#define UART_PROTOCOL_H

#include <Arduino.h>
#include "telemetry_types.h"

#define START_BYTE 0xAA
#define END_BYTE   0x03

#define MSG_ACK        0x03
#define MSG_NACK       0x04
#define MSG_TELEMETRY  0x10

#define MAX_PAYLOAD_SIZE 32

typedef struct
{
    uint8_t msgType;
    uint8_t payloadLength;
    uint8_t payload[MAX_PAYLOAD_SIZE];

} UARTPacket_t;

void UART_PROTOCOL_Init(void);

uint8_t UART_PROTOCOL_CalculateChecksum(uint8_t* payload,
                                        uint8_t length);

void UART_PROTOCOL_SendPacket(uint8_t msgType,
                              uint8_t* payload,
                              uint8_t length);

bool UART_PROTOCOL_ReadPacket(UARTPacket_t* packet);

void UART_PROTOCOL_SendACK(void);

void UART_PROTOCOL_SendNACK(void);

void UART_PROTOCOL_SendTelemetry(
    TelemetryPacket_t* telemetryData);

#endif