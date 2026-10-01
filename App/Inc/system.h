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
  uint8_t time_updated;
  uint32_t uptime_ms;
  uint32_t max_loop_time;
  uint32_t min_loop_time;
  float average_loop_time;
} SystemTime_t;

typedef struct {
  uint8_t logging;
  uint8_t debugging;
  uint8_t editing;
} SystemFlags_t;

typedef struct {
  AnalyticsData_t analytics;
  DiagnosticsData_t diagnostics;
  TelemetryStatus_t sensor_status;
  TelemetryData_t telemetry;
  SystemTime_t time;
  warning_flags_t warnings;
  SystemFlags_t flags;
} System_Context_t;

void System_Context_Init(void);

System_Context_t *System_GetContext(void);
/**
 * @brief Get the current time and date.
 * @return A pointer to a timestamp_t structure containing the current time and
 * date.
 */
timestamp_t *System_GetCurrentTime(void);

/**
 * @brief Set the time update flag.
 * @return HAL status.
 */
void System_SetTimeUpdateFlag(void);

/**
 * @brief Clear the time update flag.
 */
void System_ClearTimeUpdateFlag(void);

/**
 * @brief Check if the time update flag is set.
 * @return 1 if the flag is set, 0 otherwise.
 */
uint8_t System_IsTimeUpdateFlagSet(void);

SystemTime_t System_GetTime(void);

void System_StartTimeStatistics(void);

void System_UpdateTimeStatistics(void);

uint8_t System_IsDebugging(void);
void System_ToggleDebugging(void);

#endif /* INC_SYSTEM_H_ */
