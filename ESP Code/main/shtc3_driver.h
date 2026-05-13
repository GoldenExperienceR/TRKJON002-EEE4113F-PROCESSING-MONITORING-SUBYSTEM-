#ifndef SHTC3_DRIVER_H
#define SHTC3_DRIVER_H

#include <Arduino.h>

bool SHTC3_Init(void);

bool SHTC3_ReadData(float* temperature, float* humidity);

bool SHTC3_Wake(void);

bool SHTC3_Sleep(void);

#endif