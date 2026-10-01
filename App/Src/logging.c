/*
 * logging.c
 *
 *  Created on: 29 Sept 2026
 *      Author: Rosso
 */

#include "logging.h"
#include "analytics.h"
#include "diagnostics.h"
#include "rtc.h"
#include "system.h"
#include "telemetry.h"
#include "utils.h"
#include <stdio.h>

static volatile uint8_t log_update_flag = 0;

static void log_system_data(void);

void Logging_Process(void) {
  if (!Logging_IsLoggingUpdate()) {
    return;
  }

  Logging_ClearLoggingUpdate();

  if (!System_IsLoggingEnabled()) {
    return;
  }

  log_system_data();
}

void Logging_SetLoggingUpdate(void) { log_update_flag = 1; }

void Logging_ClearLoggingUpdate(void) { log_update_flag = 0; }

uint8_t Logging_IsLoggingUpdate(void) { return log_update_flag; }

static void log_system_data(void) {
  const timestamp_t timestamp = System_GetCurrentTime();
  const TelemetryData_t telemetry = Telemetry_GetSensorData();

  char buffer[128];
  const int written = snprintf(
      buffer, sizeof(buffer),
      "20%02u/%02u/%02u %02u:%02u:%02u, Light: %u lux, "
      "Temperature: %02d.%01d C\n",
      timestamp.date.Year, timestamp.date.Month, timestamp.date.Date,
      timestamp.time.Hours, timestamp.time.Minutes, timestamp.time.Seconds,
      telemetry.light_level, utils_split_float(telemetry.temperature, 1).whole,
      utils_split_float(telemetry.temperature, 1).fraction);

  if (written < 0 || (size_t)written >= sizeof(buffer)) {
    Diagnostics_TransmitData("Error: Log record formatting failed\n");
    return;
  }

  Diagnostics_TransmitData(buffer);
}
