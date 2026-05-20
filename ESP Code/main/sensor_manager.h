#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "telemetry_types.h"


void SensorManager_Init(void);

void SensorManager_Update(void);

TelemetryPacket_t* SensorManager_GetTelemetry(void);

bool SensorManager_TemperatureAgreementCheck(
    float toleranceC);

#endif