#include "alert_manager.h"

#include <math.h>

#include "sensor_manager.h"
#include "firmware_config.h"

static bool alertActive = false;

void AlertManager_Init(void)
{
    alertActive = false;
}

void AlertManager_Update(void)
{
    TelemetryPacket_t* telemetry =
        SensorManager_GetTelemetry();

    bool temperatureAlert = false;

    bool humidityAlert = false;

    bool voltageAlert = false;

    bool tiltAlert = false;

    bool shockAlert = false;

    // =====================================================
    // TEMPERATURE ALERT
    // =====================================================

    if(telemetry->ds18b20Temp_C >
       TEMP_ALERT_THRESHOLD_C)
    {
        temperatureAlert = true;
    }

    // =====================================================
    // HUMIDITY ALERT
    // =====================================================

    if(telemetry->humidity_percent >
       HUMIDITY_ALERT_THRESHOLD_PERCENT)
    {
        humidityAlert = true;
    }

    // =====================================================
    // LOW VOLTAGE ALERT
    // =====================================================

    if(telemetry->busVoltage_V <
       BUS_VOLTAGE_LOW_THRESHOLD_V)
    {
        voltageAlert = true;
    }

    // =====================================================
    // TILT ALERT
    // =====================================================

    if(fabs(telemetry->accelZ_g) <
       IMU_TILT_ALERT_THRESHOLD_G)
    {
        tiltAlert = true;
    }

    // =====================================================
    // SHOCK ALERT
    // =====================================================

    float accelMagnitude =
        sqrt(
            telemetry->accelX_g *
            telemetry->accelX_g +

            telemetry->accelY_g *
            telemetry->accelY_g +

            telemetry->accelZ_g *
            telemetry->accelZ_g);

    if(accelMagnitude >
       IMU_SHOCK_ALERT_THRESHOLD_G)
    {
        shockAlert = true;
    }

    // =====================================================
    // FINAL ALERT DECISION
    // =====================================================

    if(temperatureAlert ||
       humidityAlert   ||
       voltageAlert    ||
       tiltAlert       ||
       shockAlert)
    {
        alertActive = true;
    }
    else
    {
        alertActive = false;
    }
}

bool AlertManager_IsAlertActive(void)
{
    return alertActive;
}
