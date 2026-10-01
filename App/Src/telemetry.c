/*
 * telemetry.c
 *
 *  Created on: 29 Sept 2026
 *      Author: Rosso
 */

#include "telemetry.h"
#include "system.h"

static volatile uint8_t temperature_update_flag = 0;

void Telemetry_Init(void) {
  System_Context_t *sys = System_GetContext();

  fuel_data_t fuel = {.fuel_level = 5.0f, .odometer = 48.6f};
  acceleration_data_t accel = {
      .x = 0.0f, .y = 0.0f, .z = 0.0f, .magnitude = 0.3f};
  location_t loc = {.latitude = -34.8325,
                    .longitude = 20.023056,
                    .heading = 45,
                    .speed = 40.0f};

  // Populate the master system context directly
  sys->telemetry.distance = 12.5f;
  sys->telemetry.temperature = 25.6f;
  sys->telemetry.light_level = 1253;
  sys->telemetry.acceleration = accel;
  sys->telemetry.location = loc;
  sys->telemetry.fuel_data = fuel;

  temperature_update_flag = 0;
}

void Telemetry_Process(void) {}

TelemetryData_t Telemetry_GetSensorData(void) {
  System_Context_t *sys = System_GetContext();
  TelemetryData_t data = sys->telemetry;
  return data;
}

TelemetryStatus_t Telemetry_GetSensorStatus(void) {
  System_Context_t *sys = System_GetContext();

  return sys->sensor_status;
}