#ifndef FIRMWARE_CONFIG_H
#define FIRMWARE_CONFIG_H

/*====================================
  TIMING CONFIGURATION
====================================*/

#define SENSOR_POLL_INTERVAL_MS      1000

#define NORMAL_TX_INTERVAL_MS        5000

#define ALERT_TX_INTERVAL_MS         1000

#define STORAGE_INTERVAL_MS          10000


/*====================================
  ALERT THRESHOLDS
====================================*/

#define TEMP_ALERT_THRESHOLD         70.0

#define TEMP_CLEAR_THRESHOLD         65.0


/*====================================
  GPIO PINS
====================================*/

#define ALERT_PIN                    4

#define PWR_STATE_PIN                5

#endif