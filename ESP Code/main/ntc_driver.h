#ifndef NTC_DRIVER_H
#define NTC_DRIVER_H

bool NTC_Init(void);

bool NTC_ReadTemperature1(
    float* temperature
);

bool NTC_ReadTemperature2(
    float* temperature
);

#endif