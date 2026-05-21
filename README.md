# TRKJON002-EEE4113F-PROCESSING/MONITORING-SUBYSTEM

**The respository provides all the code and documentation required to recreate the processing &amp; monitoring subsystem developed for the eee4113f design project. It is apart  of a larger condition monitoring and alert system. **


The table below summarises firmware architecture implementation.

| Layer                 | File Name                                                 | Purpose                                                                                                     |
| --------------------- | --------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------- |
| **Application Layer** | `main.ino`                                                | Main firmware entry point containing initialization and the primary execution loop.                         |
| **Application Layer** | `state_machine.cpp` / `state_machine.h`                   | Implements firmware state transitions and overall operational logic.                                        |
| **Application Layer** | `communication_firmware.cpp` / `communication_firmware.h` | Implements telemetry reception, packet decoding, and live telemetry visualization on a communication ESP. |
| **Manager Layer**     | `sensor_manager.cpp` / `sensor_manager.h`                 | Handles sensor polling, aggregation, and coordination.                                                      |
| **Manager Layer**     | `alert_manager.cpp` / `alert_manager.h`                   | Manages alert generation, thresholds, and notification logic.                                               |
| **Manager Layer**     | `fault_manager.cpp` / `fault_manager.h`                   | Detects, logs, and manages system faults and abnormal conditions.                                           |
| **Manager Layer**     | `communication_manager.cpp` / `communication_manager.h`   | Coordinates subsystem and external communication handling.                                                  |
| **Manager Layer**     | `storage_manager.cpp` / `storage_manager.h`               | Handles data logging and SD card storage operations.                                                        |
| **Manager Layer**     | `watchdog_manager.cpp` / `watchdog_manager.h`             | Manages watchdog timer servicing and firmware recovery mechanisms.                                          |
| **Driver Layer**      | `uart_driver.cpp` / `uart_driver.h`                       | Low-level UART peripheral interface and serial communication handling.                                      |
| **Driver Layer**      | `sd_driver.cpp` / `sd_driver.h`                           | Low-level SD card communication and file access interface.                                                  |
| **Driver Layer**      | `rtc_driver.cpp` / `rtc_driver.h`                         | Real-time clock interface for timestamping and time synchronization.                                        |
| **Driver Layer**      | `ina219_driver.cpp` / `ina219_driver.h`                   | Interface driver for current and voltage sensing measurements.                                              |
| **Driver Layer**      | `shtc3_driver.cpp` / `shtc3_driver.h`                     | Interface driver for humidity and ambient temperature measurements.                                         |
| **Driver Layer**      | `ds18b20_driver.cpp` / `ds18b20_driver.h`                 | Interface driver for external temperature sensing.                                                          |
| **Driver Layer**      | `imu_driver.cpp` / `imu_driver.h`                         | Interface driver for inertial measurement sensing and motion acquisition using the LSM9DS1 IMU.             |
| **Protocol Layer**    | `uart_protocol.cpp` / `uart_protocol.h`                   | Implements UART packet formatting, parsing, and protocol handling.                                          |
| **Protocol Layer**    | `telemetry_types.h`                                       | Defines telemetry packet structures and shared communication data types.                                    |
| **System Layer**      | `firmware_config.h`                                       | Centralized firmware configuration constants and compile-time settings.                                     |
| **System Layer**      | `firmware_types.h`                                        | Defines shared firmware data structures and type definitions.                                               |
| **System Layer**      | `system_data.cpp` / `system_data.h`                       | Stores and manages globally shared system state and runtime data.                                           |

The tables below give information on the statemachine impleantation seen in the apllication layer. This defines the operational logic of the entire system. 

| State    | Description                                                                                                                                                                  |
| -------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `INIT`   | System startup and self-test state. Initializes sensors, RTC, SD card, UART interfaces, timers, and system thresholds while determining available subsystem functionality.   |
| `NORMAL` | Normal operating mode where sensors are periodically sampled, thresholds evaluated, telemetry prepared, and storage requests generated.                                      |
| `ALERT`  | Hazard operating state entered when threshold violations or external power alerts are detected. Polling and telemetry rates are increased while alert signalling is enabled. |
| `ERROR`  | Fault-management state entered after critical initialization or runtime failures. Fault flags are recorded while recovery and degraded operation are evaluated.              |
| `IDLE`   | Low-activity waiting state between scheduled tasks to reduce unnecessary processing and power consumption.                                                                   |

| Transition | From     | To       | Condition                                                            |
| ---------- | -------- | -------- | -------------------------------------------------------------------- |
| `T0`       | —        | `INIT`   | Subsystem powered ON.                                                |
| `T1`       | `INIT`   | `NORMAL` | Initialization complete and minimum required subsystems operational. |
| `T2`       | `INIT`   | `ERROR`  | Critical initialization failure preventing useful operation.         |
| `T3`       | `NORMAL` | `ALERT`  | Hazard threshold exceeded or external `PWR_STATE` asserted.          |
| `T4`       | `NORMAL` | `IDLE`   | No scheduled tasks pending before next polling interval.             |
| `T5`       | `IDLE`   | `NORMAL` | `IDLE` period expired or external `PWR_STATE` asserted.              |
| `T6`       | `ALERT`  | `NORMAL` | Hazard conditions cleared.                                           |
| `T7`       | `NORMAL` | `ERROR`  | Critical runtime fault detected.                                     |
| `T8`       | `ALERT`  | `ERROR`  | Critical runtime fault detected during alert operation.              |
| `T9`       | `ERROR`  | `NORMAL` | Recovery successful and degraded operation permitted.                |
