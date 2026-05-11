#include "watchdog_manager.h"

#include <Arduino.h>
#include <esp_task_wdt.h>

#define WATCHDOG_TIMEOUT_SECONDS 5

/*==================================================
INITIALIZATION
==================================================*/

void WatchdogManager_Init(void)
{
    esp_task_wdt_config_t watchdogConfig =
    {
        .timeout_ms = WATCHDOG_TIMEOUT_SECONDS * 1000,

        .idle_core_mask = (1 << portNUM_PROCESSORS) - 1,

        .trigger_panic = true
    };

    esp_task_wdt_init(&watchdogConfig);

    esp_task_wdt_add(NULL);

    Serial.println(
        "Watchdog Initialized");
}

/*==================================================
RESET WATCHDOG
==================================================*/

void WatchdogManager_Reset(void)
{
    esp_task_wdt_reset();
    
}

