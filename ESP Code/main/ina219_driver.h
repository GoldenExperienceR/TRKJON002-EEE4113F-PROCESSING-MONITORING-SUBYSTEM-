#ifndef INA219_DRIVER_H
#define INA219_DRIVER_H

bool INA219_Init(void);

bool INA219_ReadData(
    float* busVoltage,
    float* shuntVoltage,
    float* loadVoltage,
    float* current,
    float* power
);

#endif