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
static volatile uint8_t logging_enabled = 1;

static uint8_t sd_card_status = 1; // 1: OK, 0: Not OK
static uint8_t mpu6050_status = 1; // 1: OK,
static uint8_t gps_status = 1;     // 1: OK, 0: Not OK

extern UART_HandleTypeDef huart2;
extern RTC_HandleTypeDef hrtc;

RTC_TimeTypeDef gTime = {0};
RTC_DateTypeDef gDate = {0};

void Logging_Process(void) {
  if (log_update_flag) {
    log_update_flag = 0;
    System_Context_t *sys = System_GetContext();

    if (!sys)
      return;

    if (sys->flags.logging) {
      const timestamp_t *current_time = System_GetCurrentTime();
      const TelemetryData_t *sensor_data = &(sys->telemetry);

      char buffer[64];
      snprintf(buffer, sizeof(buffer),
               "20%02d/%02d/%02d %02d:%02d:%02d, %04d, %03.1f\n",
               current_time->date.Year, current_time->date.Month,
               current_time->date.Date, current_time->time.Hours,
               current_time->time.Minutes, current_time->time.Seconds,
               sensor_data->light_level, sensor_data->temperature);

      Diagnostics_TransmitData(buffer);
    }
  }
}

void Logging_SetLogUpdateFlag(void) { log_update_flag = 1; }

void Logging_ClearLogUpdateFlag(void) { log_update_flag = 0; }

uint8_t Logging_IsLogUpdateFlagSet(void) { return log_update_flag; }

void Logging_EnableLogging(void) { logging_enabled = 1; };
void Logging_DisableLogging(void) { logging_enabled = 0; };
uint8_t Logging_IsLoggingEnabled(void) { return logging_enabled; };

uint8_t Logging_SDCardStatus(void) { return sd_card_status; };
uint8_t Logging_MPU6050Status(void) { return mpu6050_status; };
uint8_t Logging_GPSStatus(void) { return gps_status; };
