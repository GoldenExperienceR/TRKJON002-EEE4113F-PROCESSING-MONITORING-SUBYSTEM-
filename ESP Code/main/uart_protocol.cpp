#include "uart_protocol.h"
#include "uart_driver.h"

void UART_PROTOCOL_Init(void)
{
    UART_HAL_Init();
}

uint8_t UART_PROTOCOL_CalculateChecksum(uint8_t* payload,
                                        uint8_t length)
{
    uint8_t checksum = 0;

    for(int i = 0; i < length; i++)
    {
        checksum ^= payload[i];
    }

    return checksum;
}

void UART_PROTOCOL_SendPacket(uint8_t msgType,
                              uint8_t* payload,
                              uint8_t length)
{
    uint8_t checksum =
        UART_PROTOCOL_CalculateChecksum(payload, length);

    UART_HAL_SendByte(START_BYTE);

    UART_HAL_SendByte(msgType);

    UART_HAL_SendByte(length);

    UART_HAL_SendBuffer(payload, length);

    UART_HAL_SendByte(checksum);

    UART_HAL_SendByte(END_BYTE);
}

bool UART_PROTOCOL_ReadPacket(UARTPacket_t* packet)
{
    if(UART_HAL_ByteAvailable() < 5)
    {
        return false;
    }

    if(UART_HAL_ReadByte() != START_BYTE)
    {
        return false;
    }

    packet->msgType = UART_HAL_ReadByte();

    packet->payloadLength = UART_HAL_ReadByte();

    if(packet->payloadLength > MAX_PAYLOAD_SIZE)
    {
        UART_HAL_Flush();
        return false;
    }

    uint8_t checksum = 0;

    for(int i = 0; i < packet->payloadLength; i++)
    {
        packet->payload[i] = UART_HAL_ReadByte();

        checksum ^= packet->payload[i];
    }

    uint8_t receivedChecksum =
        UART_HAL_ReadByte();

    uint8_t endByte =
        UART_HAL_ReadByte();

    if(endByte != END_BYTE)
    {
        return false;
    }

    if(checksum != receivedChecksum)
    {
        return false;
    }

    return true;
}

void UART_PROTOCOL_SendACK(void)
{
    uint8_t ackPayload = 0x06;

    UART_PROTOCOL_SendPacket(MSG_ACK,
                             &ackPayload,
                             1);
}

void UART_PROTOCOL_SendNACK(void)
{
    uint8_t nackPayload = 0x15;

    UART_PROTOCOL_SendPacket(MSG_NACK,
                             &nackPayload,
                             1);
}

void UART_PROTOCOL_SendTelemetry(
    TelemetryPacket_t* telemetryData)
{
    UART_PROTOCOL_SendPacket(
        MSG_TELEMETRY,
        (uint8_t*)telemetryData,
        sizeof(TelemetryPacket_t));
}
