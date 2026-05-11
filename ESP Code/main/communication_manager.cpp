#include "communication_manager.h"
#include "sensor_manager.h"
#include "uart_protocol.h"
#include "firmware_config.h"
#include "state_machine.h"

#include <Arduino.h>

static uint32_t lastTxTime = 0;

void CommunicationManager_Init(void)
{
    Serial.println("Communication Manager Initialized");
}

void CommunicationManager_Update(void)
{
    uint32_t currentTime = millis();

    if (currentTime - lastTxTime >= NORMAL_TX_INTERVAL_MS)
    {
        lastTxTime = currentTime;

        UART_PROTOCOL_SendTelemetry(
            SensorManager_GetTelemetry());

        Serial.println("Telemetry transmitted");

        UARTResponse_t response =
            UART_PROTOCOL_WaitForResponse(5000);

        switch(response)
        {
            case UART_RESPONSE_ACK:

                Serial.println("ACK received");
                break;

            case UART_RESPONSE_NACK:

                Serial.println("NACK received");
                break;

            case UART_RESPONSE_TIMEOUT:

                Serial.println("Response timeout");
                break;

            case UART_RESPONSE_INVALID:

                Serial.println("Invalid response");
                break;

            default:
                break;
        }
    }
}