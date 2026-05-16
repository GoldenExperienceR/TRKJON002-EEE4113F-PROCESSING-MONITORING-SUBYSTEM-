#ifndef FIRMWARE_CONFIG_H
#define FIRMWARE_CONFIG_H

// =====================================================
// POWER STATE MONITORING
// =====================================================
#define PWR_STATE_PIN 4


// =====================================================
// WATCH DOG TIMEOUT 
// =====================================================
#define WATCHDOG_TIMEOUT_SECONDS 10000

// =====================================================
// SLEEP MODE 
// =====================================================

    #define IDLE_ENTRY_INTERVAL_MS    30000

    #define IDLE_DURATION_MS          5000

// =====================================================
// UART CONFIGURATION
// =====================================================

// Uncomment for USB Serial debugging
//#define USE_USB_UART

// =====================================================
// UART SETTINGS
// =====================================================

#define UART_BAUD_RATE 115200

#define UART2_RX_PIN 16
#define UART2_TX_PIN 17


// =====================================================
// SENSOR POLLING
// =====================================================

#define NORMAL_SENSOR_POLL_INTERVAL_MS      2000

#define ALERT_SENSOR_POLL_INTERVAL_MS        500

// =====================================================
// UART TELEMETRY
// =====================================================

#define NORMAL_TX_INTERVAL_MS               5000

#define ALERT_TX_INTERVAL_MS                1000

// =====================================================
// STORAGE
// =====================================================

#define STORAGE_LOG_INTERVAL_MS            5000

// =====================================================
// TEMPERATURE THRESHOLDS
// =====================================================

#define TEMP_ALERT_THRESHOLD_C              50.0f

#define TEMP_ALERT_CLEAR_THRESHOLD_C        45.0f

// =====================================================
// HUMIDITY THRESHOLDS
// =====================================================

#define HUMIDITY_ALERT_THRESHOLD_PERCENT    85.0f

#define HUMIDITY_ALERT_CLEAR_PERCENT        80.0f

// =====================================================
// BUS VOLTAGE THRESHOLDS
// =====================================================

#define BUS_VOLTAGE_LOW_THRESHOLD_V         10.5f

#define BUS_VOLTAGE_CLEAR_THRESHOLD_V       11.0f

// =====================================================
// IMU TILT THRESHOLDS
// =====================================================
// Measured using accel vector orientation.
//
// Example:
// Upright Z-axis ≈ 1 g
// Tilt/fall reduces Z contribution significantly.
// Thresholds should later be experimentally tuned.
//

#define IMU_TILT_ALERT_THRESHOLD_G           0.50f

#define IMU_TILT_CLEAR_THRESHOLD_G           0.70f

// =====================================================
// IMU SHOCK THRESHOLDS
// =====================================================
// Sudden acceleration magnitude spike.
//
// Units: g
//

#define IMU_SHOCK_ALERT_THRESHOLD_G          2.50f

#define IMU_SHOCK_CLEAR_THRESHOLD_G          1.50f

#endif

