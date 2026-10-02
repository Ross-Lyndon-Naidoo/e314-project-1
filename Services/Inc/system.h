/*
 * system.h
 *
 *  Created on: 30 Sept 2026
 *      Author: Rosso
 */

#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

#include "analytics.h"
#include "diagnostics.h"
#include "system_types.h"
#include "telemetry.h"
#include "ui.h"
#include <stdint.h>

typedef struct {
  timestamp_t current_time;
  uint32_t uptime_ms;
  uint32_t loop_time_min_us;
  uint32_t loop_time_max_us;
  uint32_t loop_time_avg_us;
} SystemTime_t;

typedef struct {
  uint8_t time_updated;
  uint8_t debugging;
  uint8_t logging;
  uint8_t editing;
  warning_flags_t warnings;

} SystemStatus_t;
typedef struct {
  AnalyticsData_t analytics;
  DiagnosticsData_t diagnostics;
  TelemetryStatus_t sensor_status;
  TelemetryData_t telemetry;
  SystemStatus_t status;
  SystemTime_t time;
  warning_flags_t warnings;
} System_Context_t;

void System_Init(void);

void System_LoopBegin(void);
void System_LoopEnd(void);

System_Context_t *System_GetContext(void);

/**
 * @brief Get the current time and date.
 * @return A pointer to a timestamp_t structure containing the current time and
 * date.
 */
timestamp_t System_GetCurrentTime(void);

/**
 * @brief Set the time update flag.
 * @return HAL status.
 */
void System_SetTimeUpdateFlag(void);

/**
 * @brief Check if the time update flag is set.
 * @return 1 if the flag is set, 0 otherwise.
 */
uint8_t System_IsTimeUpdateFlagSet(void);

/**
 * @brief Get the system time statistics.
 * @return A SystemTime_t structure containing the system time statistics.
 */
SystemTime_t System_GetTime(void);

uint32_t System_GetUptimeMs(void);
uint8_t System_IsDebugging(void);
void System_ToggleDebugging(void);

/**
 * @brief Check if logging is enabled.
 * @return 1 if logging is enabled, 0 otherwise.
 */
uint8_t System_IsLoggingEnabled(void);

/**
 * @brief Enable or disable logging.
 * @param enabled 1 to enable logging, 0 to disable.
 */
void System_SetLoggingEnabled(uint8_t enabled);

uint8_t System_IsEditing(void);
void System_SetEditing(uint8_t editing);

timestamp_t System_GetCurrentTime(void);

TelemetryStatus_t System_GetSensorStatus(void);
SystemStatus_t System_GetStatus(void);

#endif /* INC_SYSTEM_H_ */
