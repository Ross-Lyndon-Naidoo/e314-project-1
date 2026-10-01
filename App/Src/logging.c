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
#include "usart.h"
#include <stdio.h>
#include <string.h>

static volatile uint8_t log_update_flag = 0;

extern UART_HandleTypeDef huart2;
extern RTC_HandleTypeDef hrtc;

void log_system_data(void);

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

void log_system_data(void) {
  const timestamp_t timestamp = System_GetCurrentTime();
  TelemetryData_t telemetry = Telemetry_GetSensorData();

  char buffer[128];
  snprintf(buffer, sizeof(buffer),
           "20%02u/%02u/%02u %02u:%02u:%02u, %u, %.1f\n", timestamp.date.Year,
           timestamp.date.Month, timestamp.date.Date, timestamp.time.Hours,
           timestamp.time.Minutes, timestamp.time.Seconds,
           telemetry.light_level, telemetry.temperature);

  Diagnostics_TransmitBlocking(buffer);
}