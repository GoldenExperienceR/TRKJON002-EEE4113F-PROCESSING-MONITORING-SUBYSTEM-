#ifndef SHTC3_DRIVER_H
#define SHTC3_DRIVER_H

bool SHTC3_Init(void);

bool SHTC3_ReadData(float* temperature, float* humidity);

#endif