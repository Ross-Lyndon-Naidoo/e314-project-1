/*
 * analytics.c
 *
 *  Created on: 29 Sept 2026
 *      Author: Rosso
 */

#include "analytics.h"
#include "diagnostics.h"
#include "system.h"
#include "telemetry.h"
#include <stdint.h>
#include <stdio.h>

extern RTC_HandleTypeDef hrtc;

static volatile uint8_t fuel_efficiency_flag = 0;
static volatile uint8_t warning_flag = 0;

static fuel_efficiency_t calculate_fuel_efficiency(float fuel_level,
                                                   float odometer);
static void update_fuel_efficiency(void);

// ==========================================
// Public Functions
// ==========================================
void Analytics_Init(void) {
  System_Context_t *sys = System_GetContext();

  sys->analytics = (AnalyticsData_t){0};

  sys->analytics.fuel_data = (fuel_data_t){
      .fuel_level = 5.0f,
      .odometer = 48.6f,
  };

  sys->analytics.fuel_efficiency = calculate_fuel_efficiency(
      sys->analytics.fuel_data.fuel_level, sys->analytics.fuel_data.odometer);

  if (System_IsDebugging()) {
    Diagnostics_TransmitBlocking("Analytics Initialized\n");
  }
}

void Analytics_Process(void) {
  if (fuel_efficiency_flag) {
    fuel_efficiency_flag = 0U;
    update_fuel_efficiency();
  }
}

AnalyticsData_t Analytics_GetAnalyticsData(void) {
  const System_Context_t *sys = System_GetContext();
  return sys->analytics;
}

warning_flags_t Analytics_GetWarnings(void) {
  SystemStatus_t status = System_GetStatus();
  return status.warnings;
}

void Analytics_SetWarnings(warning_flags_t flags) {
  System_Context_t *sys = System_GetContext();
  sys->warnings = flags;
}

void Analytics_SetFuelEfficiency(fuel_efficiency_t efficiency) {
  System_Context_t *sys = System_GetContext();
  sys->analytics.fuel_efficiency = efficiency;
}

// ==========================================
// PRIVATE HELPER FUNCTIONS
// ==========================================
static fuel_efficiency_t calculate_fuel_efficiency(float fuel_level,
                                                   float odometer) {
  fuel_efficiency_t efficiency;

  if (fuel_level > 0.0f && odometer > 0.0f) {
    efficiency.km_per_liter = odometer / fuel_level;
    efficiency.liters_per_100km = (fuel_level / odometer) * 100.0f;
  } else {
    efficiency.km_per_liter = 0.0f;
    efficiency.liters_per_100km = 0.0f;
  }
  return efficiency;
}

static void update_fuel_efficiency(void) {
  const TelemetryData_t telemetry = Telemetry_GetSensorData();
  System_Context_t *sys = System_GetContext();

  sys->analytics.fuel_data = telemetry.fuel_data;
  sys->analytics.fuel_efficiency = calculate_fuel_efficiency(
      telemetry.fuel_data.fuel_level, telemetry.fuel_data.odometer);
}