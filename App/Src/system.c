/*
 * system.c
 *
 *  Created on: 30 Sept 2026
 *      Author: Rosso
 */

#include "system.h"
#include "diagnostics.h"
#include "tim.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
// The single global instance of your entire application state
extern TIM_HandleTypeDef htim7;
extern UART_HandleTypeDef huart2;

static System_Context_t master_system;

static uint32_t boot_tick;
static uint32_t loop_start_tick;

timestamp_t get_current_time(void);

void System_Init(void) {

  boot_tick = HAL_GetTick();

  master_system = (System_Context_t){0};
  master_system.time.loop_time_max_us = UINT32_MAX;
  master_system.time.current_time = get_current_time();

  master_system.status.debugging = 1;
  master_system.status.logging = 1;

  if (System_IsDebugging())
    Diagnostics_TransmitBlocking("System Initialized\n");
  // Sub-module initializations can populate their respective structs here
}

void System_LoopBegin(void) { loop_start_tick = HAL_GetTick(); }

void System_LoopEnd(void) {
  const uint32_t elapsed_ms = HAL_GetTick() - loop_start_tick;

  if (elapsed_ms > master_system.time.loop_time_max_us) {
    master_system.time.loop_time_max_us = elapsed_ms;
  }

  if (elapsed_ms < master_system.time.loop_time_min_us) {
    master_system.time.loop_time_min_us = elapsed_ms;
  }

  master_system.time.loop_time_avg_us =
      0.99f * master_system.time.loop_time_avg_us + 0.01f * (float)elapsed_ms;
}

System_Context_t *System_GetContext(void) { return &master_system; }

uint32_t System_GetUptimeMs(void) { return HAL_GetTick() - boot_tick; }

// Global accessor so modules can grab context safely

void System_SetTimeUpdateFlag(void) { master_system.status.time_updated = 1; }

uint8_t System_IsTimeUpdateFlagSet(void) {
  return master_system.status.time_updated;
}

timestamp_t System_GetCurrentTime(void) {
  master_system.time.current_time = get_current_time();
  return master_system.time.current_time;
}

uint8_t System_IsDebugging(void) { return master_system.status.debugging; }

void System_ToggleDebugging(void) {
  master_system.status.debugging = !master_system.status.debugging;
}

SystemTime_t System_GetTime(void) { return master_system.time; }

timestamp_t get_current_time(void) {
  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;
  timestamp_t current_time;

  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

  current_time.time = sTime;
  current_time.date = sDate;
  return current_time;
}

uint8_t System_IsLoggingEnabled(void) { return master_system.status.logging; }

void System_SetLoggingEnabled(uint8_t enabled) {
  master_system.status.logging = enabled ? 1U : 0U;
}

uint8_t System_IsEditing(void) { return master_system.status.editing; }

void System_SetEditing(uint8_t editing) {
  master_system.status.editing = editing;
}

SystemStatus_t System_GetStatus(void) { return master_system.status; }