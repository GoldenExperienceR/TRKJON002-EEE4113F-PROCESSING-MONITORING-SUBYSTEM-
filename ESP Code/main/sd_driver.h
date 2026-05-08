#ifndef SD_DRIVER_H
#define SD_DRIVER_H

#include <stdint.h>

/*====================================
  PUBLIC FUNCTIONS
====================================*/

bool SD_Init(void);

bool SD_WriteLogEntry(const char* logEntry);

#endif