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
static volatile uint8_t time_update_flag = 0;
static volatile uint8_t editing_flag = 0;

static volatile uint8_t fuel_efficiency_flag = 0;

static volatile uint8_t warning_flag = 0;

void calculate_fuel_efficiency(void);
void update_time(void);

// ==========================================
// Public Functions
// ==========================================
void Analytics_Init(void) {
  System_Context_t *sys = System_GetContext();
  sys->analytics.fuel_data.fuel_level = 5.0f;
  sys->analytics.fuel_data.odometer = 48.6f;

  calculate_fuel_efficiency();
  if (System_IsDebugging())
    Diagnostics_TransmitBlocking("Analytics Initialized\n");
}

void Analytics_Process(void) {
  if (fuel_efficiency_flag) {
    fuel_efficiency_flag = 0;
    calculate_fuel_efficiency();
  }

  if (warning_flag) {
    warning_flag = 0;
    // Process warning flags
  }
}

AnalyticsData_t Analytics_GetAnalyticsData(void) {
  System_Context_t *sys = System_GetContext();
  return sys->analytics;
}

// Fuel efficiency
fuel_efficiency_t Analytics_GetFuelEfficiency(void) {
  System_Context_t *sys = System_GetContext();
  const fuel_efficiency_t efficiency = sys->analytics.fuel_efficiency;
  return efficiency;
}

void Analytics_SetFuelEfficiency(fuel_efficiency_t efficiency) {
  System_Context_t *sys = System_GetContext();
  sys->analytics.fuel_efficiency = efficiency;
}

// Editing state management
void Analytics_SetEditing(void) {
  System_Context_t *sys = System_GetContext();
  sys->analytics.is_editing = 1;
}
void Analytics_ClearEditing(void) {
  System_Context_t *sys = System_GetContext();
  sys->analytics.is_editing = 0;
}

uint8_t Analytics_IsEditing(void) {
  System_Context_t *sys = System_GetContext();
  return sys->analytics.is_editing;
}

// Warning flags
warning_flags_t Analytics_GetWarningFlags(void) {
  System_Context_t *sys = System_GetContext();
  return sys->warnings;
}

// ==========================================
// PRIVATE HELPER FUNCTIONS
// ==========================================
void calculate_fuel_efficiency(void) {
  System_Context_t *sys = System_GetContext();

  if (sys->telemetry.fuel_data.fuel_level > 0.0f &&
      sys->telemetry.fuel_data.odometer > 0.0f) {
    sys->analytics.fuel_efficiency.km_per_liter =
        sys->telemetry.fuel_data.odometer / sys->telemetry.fuel_data.fuel_level;

    sys->analytics.fuel_efficiency.liters_per_100km =
        (sys->telemetry.fuel_data.fuel_level /
         sys->telemetry.fuel_data.odometer) *
        100.0f;
  } else {
    sys->analytics.fuel_efficiency.km_per_liter = 0.0f;
    sys->analytics.fuel_efficiency.liters_per_100km = 0.0f;
  }
}
