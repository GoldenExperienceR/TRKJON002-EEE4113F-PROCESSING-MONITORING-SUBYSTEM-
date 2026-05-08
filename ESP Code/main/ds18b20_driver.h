#ifndef DS18B20_DRIVER_H
#define DS18B20_DRIVER_H

bool DS18B20_Init(void);

bool DS18B20_ReadTemperature(
    float* temperature
);

#endif