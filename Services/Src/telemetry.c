/*
 * telemetry.c
 *
 *  Created on: 29 Sept 2026
 *      Author: Rosso
 */

#include "telemetry.h"

static TelemetryData_t telemetry_data;
static TelemetryStatus_t telemetry_status;

void Telemetry_Init(void)
{
  telemetry_data = (TelemetryData_t){
      .distance = 12.5f,
      .temperature = 25.6f,
      .light_level = 1253U,
      .acceleration = {
          .x = 0.0f,
          .y = 0.0f,
          .z = 0.0f,
          .magnitude = 0.3f,
      },
      .location = {
          .latitude = -34.8325,
          .longitude = 20.023056,
          .heading = 45U,
          .speed = 40.0f,
      },
      .fuel_data = {
          .fuel_level = 5.0f,
          .odometer = 48.6f,
      },
  };

  telemetry_status = (TelemetryStatus_t){
      .sd_card_status = 1U,
      .mpu6050_status = 1U,
      .gps_status = 1U,
  };
}

void Telemetry_Process(void)
{
}

TelemetryData_t Telemetry_GetData(void)
{
  return telemetry_data;
}

TelemetryStatus_t Telemetry_GetStatus(void)
{
  return telemetry_status;
}