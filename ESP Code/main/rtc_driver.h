#ifndef RTC_DRIVER_H
#define RTC_DRIVER_H

#include <stdint.h>

/*====================================
  PUBLIC FUNCTIONS
====================================*/

bool RTC_Init(void);

bool RTC_GetTimestamp(char* timestampBuffer);

#endif