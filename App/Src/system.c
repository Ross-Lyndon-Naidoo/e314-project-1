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

static uint8_t time_update_flag = 0;
static System_Context_t master_system;

timestamp_t get_current_time(void);

void System_Context_Init(void) {
  // Zero out master context
  master_system.time.uptime_ms = 0;
  master_system.time.max_loop_time = 0;
  master_system.time.min_loop_time = 0xFFFFFFFF;
  master_system.time.average_loop_time = 0.0f;
  master_system.time.current_time = get_current_time();

  master_system.flags.debugging = 1;
  master_system.flags.logging = 1;

  if (System_IsDebugging())
    Diagnostics_TransmitBlocking("System Initialized\n");
  // Sub-module initializations can populate their respective structs here
}

// Global accessor so modules can grab context safely
System_Context_t *System_GetContext(void) { return &master_system; }

timestamp_t *System_GetCurrentTime(void) {
  master_system.time.current_time = get_current_time();

  return &master_system.time.current_time;
}

void System_SetTimeUpdateFlag(void) { time_update_flag = 1; }

void System_ClearTimeUpdateFlag(void) { time_update_flag = 0; }

uint8_t System_IsTimeUpdateFlagSet(void) { return time_update_flag; }

uint8_t System_IsDebugging(void) { return master_system.flags.debugging; }

void System_ToggleDebugging(void) {
  master_system.flags.debugging = !master_system.flags.debugging;
}

SystemTime_t System_GetTime(void) { return master_system.time; }

void System_StartTimeStatistics(void) {
  master_system.time.uptime_ms = HAL_GetTick();

  __HAL_TIM_SET_COUNTER(&htim7, 0);
  HAL_TIM_Base_Start(&htim7);
}

void System_UpdateTimeStatistics(void) {
  HAL_TIM_Base_Stop(&htim7);

  uint32_t elapsed_time = __HAL_TIM_GET_COUNTER(&htim7);

  if (elapsed_time > master_system.time.max_loop_time) {
    master_system.time.max_loop_time = elapsed_time;
  }
  if (elapsed_time < master_system.time.min_loop_time) {
    master_system.time.min_loop_time = elapsed_time;
  }
  master_system.time.average_loop_time =
      0.99f * (float)master_system.time.average_loop_time +
      0.01f * (float)elapsed_time;
}

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
