#include "analytics.h"
#include "diagnostics.h"
#include "system.h"
#include "telemetry.h"

static AnalyticsData_t analytics_data;
static warning_flags_t analytics_warnings;

static uint8_t fuel_efficiency_update_pending;

static fuel_efficiency_t calculate_fuel_efficiency(float fuel_level,
                                                   float odometer);
static void update_fuel_efficiency(void);

void Analytics_Init(void) {
  analytics_data = (AnalyticsData_t){0};
  analytics_warnings = (warning_flags_t){0};
  fuel_efficiency_update_pending = 0U;

  update_fuel_efficiency();

  if (System_IsDebugging()) {
    Diagnostics_TransmitBlocking("Analytics Initialized\n");
  }
}

void Analytics_Process(void) {
  if (fuel_efficiency_update_pending != 0U) {
    fuel_efficiency_update_pending = 0U;
    update_fuel_efficiency();
  }

  /*
   * Warning calculations would be processed here.
   * For example:
   *
   * const TelemetryData_t telemetry = Telemetry_GetData();
   * analytics_warnings = calculate_warnings(&telemetry);
   */
}

AnalyticsData_t Analytics_GetData(void) { return analytics_data; }

warning_flags_t Analytics_GetWarnings(void) { return analytics_warnings; }

void Analytics_SetWarnings(warning_flags_t flags) {
  analytics_warnings = flags;
}

void Analytics_SetFuelEfficiency(fuel_efficiency_t efficiency) {
  analytics_data.fuel_efficiency = efficiency;
}

static void update_fuel_efficiency(void) {
  const TelemetryData_t telemetry = Telemetry_GetData();
  const fuel_data_t fuel = telemetry.fuel_data;

  analytics_data.fuel_data = fuel;
  analytics_data.fuel_efficiency =
      calculate_fuel_efficiency(fuel.fuel_level, fuel.odometer);
}

static fuel_efficiency_t calculate_fuel_efficiency(float fuel_level,
                                                   float odometer) {
  fuel_efficiency_t efficiency = {0};

  if (fuel_level > 0.0f && odometer > 0.0f) {
    efficiency.km_per_liter = odometer / fuel_level;
    efficiency.liters_per_100km = (fuel_level / odometer) * 100.0f;
  }

  return efficiency;
}