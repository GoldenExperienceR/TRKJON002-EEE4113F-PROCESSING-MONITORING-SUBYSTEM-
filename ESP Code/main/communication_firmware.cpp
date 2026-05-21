#include "communication_firmware.h"

#include <Arduino.h>
#include <string.h>

#include "uart_protocol.h"
#include "telemetry_types.h"

static TelemetryPacket_t receivedTelemetry;

static void PrintTelemetry(void);

void CommunicationSetup(void)
{
    Serial.begin(115200);

    UART_PROTOCOL_Init();

    Serial.println(
        "Communication ESP Ready");
}

void CommunicationLoop(void)
{
    UARTPacket_t packet;

    if(UART_PROTOCOL_ReadPacket(&packet))
    {
        if(packet.msgType == MSG_TELEMETRY)
        {
            if(packet.payloadLength ==
               sizeof(TelemetryPacket_t))
            {
                memcpy(
                    &receivedTelemetry,
                    packet.payload,
                    sizeof(TelemetryPacket_t));

                PrintTelemetry();
            }
            else
            {
                Serial.println(
                    "Telemetry size mismatch");
            }
        }
    }
}

static void PrintTelemetry(void)
{
    Serial.println(
        "==============================");

    Serial.print("RTC Time: ");
    Serial.println(
        receivedTelemetry.rtcTimestamp);

    Serial.print("Ambient Temp: ");
    Serial.print(
        receivedTelemetry.ambientTemp_C);
    Serial.println(" C");

    Serial.print("Humidity: ");
    Serial.print(
        receivedTelemetry.humidity_percent);
    Serial.println(" %");

    Serial.print("DS18B20 Temp: ");
    Serial.print(
        receivedTelemetry.ds18b20Temp_C);
    Serial.println(" C");

    Serial.print("NTC1 Temp: ");
    Serial.print(
        receivedTelemetry.ntc1Temp_C);
    Serial.println(" C");

    Serial.print("NTC2 Temp: ");
    Serial.print(
        receivedTelemetry.ntc2Temp_C);
    Serial.println(" C");

    Serial.print("Bus Voltage: ");
    Serial.print(
        receivedTelemetry.busVoltage_V);
    Serial.println(" V");

    Serial.print("Current: ");
    Serial.print(
        receivedTelemetry.current_mA);
    Serial.println(" mA");

    Serial.print("Power: ");
    Serial.print(
        receivedTelemetry.power_mW);
    Serial.println(" mW");

    Serial.print("Accel X: ");
    Serial.println(
        receivedTelemetry.accelX_g);

    Serial.print("Accel Y: ");
    Serial.println(
        receivedTelemetry.accelY_g);

    Serial.print("Accel Z: ");
    Serial.println(
        receivedTelemetry.accelZ_g);

    Serial.print("Gyro X: ");
    Serial.println(
        receivedTelemetry.gyroX_dps);

    Serial.print("Gyro Y: ");
    Serial.println(
        receivedTelemetry.gyroY_dps);

    Serial.print("Gyro Z: ");
    Serial.println(
        receivedTelemetry.gyroZ_dps);

    Serial.print("Health Flags: 0x");
    Serial.println(
        receivedTelemetry.systemHealthFlags,
        HEX);

    Serial.println(
        "==============================");
}