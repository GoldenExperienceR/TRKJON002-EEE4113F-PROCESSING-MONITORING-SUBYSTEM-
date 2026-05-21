#include "uart_protocol.h"
#include "uart_driver.h"

void UART_PROTOCOL_Init(void)
{
    UART_HAL_Init();
}

uint8_t UART_PROTOCOL_CalculateChecksum(const uint8_t* payload,
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
                              const uint8_t* payload,
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
    // =====================================================
    // WAIT FOR START BYTE
    // =====================================================

    while (UART_HAL_BytesAvailable() > 0)
    {
        uint8_t byteIn = UART_HAL_ReadByte();

        if (byteIn == START_BYTE)
        {
            // =================================================
            // WAIT FOR HEADER
            // =================================================

            while (UART_HAL_BytesAvailable() < 2)
            {
                // Wait for msgType + payloadLength
            }

            packet->msgType =
                UART_HAL_ReadByte();

            packet->payloadLength =
                UART_HAL_ReadByte();

            // =================================================
            // VALIDATE PAYLOAD LENGTH
            // =================================================

            if (packet->payloadLength >
                MAX_PAYLOAD_SIZE)
            {
                UART_HAL_Flush();
                return false;
            }

            // =================================================
            // WAIT FOR FULL PACKET
            // payload + checksum + end byte
            // =================================================

            while (UART_HAL_BytesAvailable() <
                   (packet->payloadLength + 2))
            {
                // Wait
            }

            // =================================================
            // READ PAYLOAD
            // =================================================

            uint8_t calculatedChecksum = 0;

            for (int i = 0;
                 i < packet->payloadLength;
                 i++)
            {
                packet->payload[i] =
                    UART_HAL_ReadByte();

                calculatedChecksum ^=
                    packet->payload[i];
            }

            // =================================================
            // READ CHECKSUM
            // =================================================

            uint8_t receivedChecksum =
                UART_HAL_ReadByte();

            // =================================================
            // READ END BYTE
            // =================================================

            uint8_t endByte =
                UART_HAL_ReadByte();

            // =================================================
            // VALIDATE END BYTE
            // =================================================

            if (endByte != END_BYTE)
            {
                return false;
            }

            // =================================================
            // VALIDATE CHECKSUM
            // =================================================

            if (receivedChecksum !=
                calculatedChecksum)
            {
                return false;
            }

            // =================================================
            // PACKET VALID
            // =================================================

            return true;
        }
    }

    return false;
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

bool UART_PROTOCOL_WaitForACK(void)
{
    UARTPacket_t packet;

    uint32_t startTime = millis();

    while ((millis() - startTime) < 1000)

    {
        if (UART_PROTOCOL_ReadPacket(&packet))
        {
            if (packet.msgType == MSG_ACK)
            {
                if (packet.payloadLength >= 1)
                {
                    if (packet.payload[0] == 0x06)
                    {
                        return true;
                    }
                }
            }
        }
    }
}
UARTResponse_t UART_PROTOCOL_WaitForResponse(
    uint32_t timeout_ms)
{
    UARTPacket_t packet;

    uint32_t startTime = millis();

    while ((millis() - startTime) < timeout_ms)
    {
        if (UART_PROTOCOL_ReadPacket(&packet))
        {
            if (packet.msgType == MSG_ACK)
            {
                return UART_RESPONSE_ACK;
            }

            else if (packet.msgType == MSG_NACK)
            {
                return UART_RESPONSE_NACK;
            }

            else
            {
                return UART_RESPONSE_INVALID;
            }
        }
    }

    return UART_RESPONSE_TIMEOUT;
}
