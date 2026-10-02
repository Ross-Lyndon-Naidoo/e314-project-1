/*
 * ui.c
 *
 *  Created on: 29 Sept 2026
 *      Author: Ross Naidoo
 */

#include "ui.h"
#include "analytics.h"
#include "led.h"
#include "logging.h"
#include "main.h"
#include "rtc.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "system.h"
#include "telemetry.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//========== Private Variables ==========
char display_buffer[3][19];

//========== Private Function Prototypes ==============
void perform_page_transition(UIPage_t target_page);
void draw_display_page_default(void);
void draw_display_page_1(void);
void draw_display_page_2(void);
void draw_display_page_3(void);
void draw_display_page_4(void);
void draw_display_page_5(void);
void draw_data_entry_page_default(void);
void draw_data_entry_page_1(void);
void draw_data_entry_page_2(void);
void draw_data_entry_page_3(void);
void draw_diagnostics_page_default(void);
void draw_diagnostics_page_1(void);
void draw_diagnostics_page_2(void);
void draw_diagnostics_page_3(void);
void draw_diagnostics_page_4(void);
void draw_warning_unsafe_driving(void);
void draw_warning_impact_detected(void);
void draw_warning_low_light(void);
void draw_warning_proximity(void);
void draw_warning_high_temperature(void);

void display_metric(char *display, size_t max_len, const char *left,
                    float number, const char *unit);
void display_string(char *display, size_t max_len, const char *str);
void draw_display_buffer(void);
void display_time(void);

typedef struct {
  void (*draw_function)(void);
} state_function_t;

static const state_function_t state_table[UI_PAGE_COUNT] = {
    [UI_PAGE_DISPLAY_DEFAULT] = {draw_display_page_default},
    [UI_PAGE_DATA_ENTRY_DEFAULT] = {draw_data_entry_page_default},
    [UI_PAGE_DIAGNOSTICS_DEFAULT] = {draw_diagnostics_page_default},
    [UI_PAGE_DISPLAY_1] = {draw_display_page_1},
    [UI_PAGE_DISPLAY_2] = {draw_display_page_2},
    [UI_PAGE_DISPLAY_3] = {draw_display_page_3},
    [UI_PAGE_DISPLAY_4] = {draw_display_page_4},
    [UI_PAGE_DISPLAY_5] = {draw_display_page_5},
    [UI_PAGE_DATA_ENTRY_1] = {draw_data_entry_page_1},
    [UI_PAGE_DATA_ENTRY_2] = {draw_data_entry_page_2},
    [UI_PAGE_DATA_ENTRY_3] = {draw_data_entry_page_3},
    [UI_PAGE_DIAGNOSTICS_1] = {draw_diagnostics_page_1},
    [UI_PAGE_DIAGNOSTICS_2] = {draw_diagnostics_page_2},
    [UI_PAGE_DIAGNOSTICS_3] = {draw_diagnostics_page_3},
    [UI_PAGE_DIAGNOSTICS_4] = {draw_diagnostics_page_4},
    [UI_PAGE_WARNING_UNSAFE_DRIVING] = {draw_warning_unsafe_driving},
    [UI_PAGE_WARNING_IMPACT_DETECTED] = {draw_warning_impact_detected},
    [UI_PAGE_WARNING_LOW_LIGHT] = {draw_warning_low_light},
    [UI_PAGE_WARNING_PROXIMITY] = {draw_warning_proximity},
    [UI_PAGE_WARNING_HIGH_TEMPERATURE] = {draw_warning_high_temperature}};

#define NUM_PAGES (sizeof(state_table) / sizeof(state_table[0]))

static const Transition_t transition_table[] = {
    {UI_PAGE_DISPLAY_DEFAULT, UI_EVENT_NAV_NEXT, UI_PAGE_DATA_ENTRY_DEFAULT},
    {UI_PAGE_DISPLAY_DEFAULT, UI_EVENT_NAV_PREV, UI_PAGE_DIAGNOSTICS_DEFAULT},
    {UI_PAGE_DATA_ENTRY_DEFAULT, UI_EVENT_NAV_NEXT,
     UI_PAGE_DIAGNOSTICS_DEFAULT},
    {UI_PAGE_DATA_ENTRY_DEFAULT, UI_EVENT_NAV_PREV, UI_PAGE_DISPLAY_DEFAULT},
    {UI_PAGE_DIAGNOSTICS_DEFAULT, UI_EVENT_NAV_NEXT, UI_PAGE_DISPLAY_DEFAULT},
    {UI_PAGE_DIAGNOSTICS_DEFAULT, UI_EVENT_NAV_PREV,
     UI_PAGE_DATA_ENTRY_DEFAULT},
};

#define NUM_TRANSITIONS (sizeof(transition_table) / sizeof(transition_table[0]))

typedef struct {
  UIPage_t current_page;
  UIPage_t previous_page;
  UIEvent_t current_event;
  uint8_t force_redraw;   // Flag to tell the OLED engine to update
  uint8_t is_initialized; // Prevents processing before UI_Init() is called
} UI_StateMachine_t;

static UI_StateMachine_t sm = {.current_page = UI_PAGE_DISPLAY_DEFAULT,
                               .previous_page = UI_PAGE_DISPLAY_DEFAULT,
                               .current_event = UI_EVENT_NONE,
                               .force_redraw = 0,
                               .is_initialized = 0};

//============== Public Functions ==============
void UI_Init(void) {
  ssd1306_Init();
  ssd1306_SetCursor(0, 0);
  ssd1306_WriteString("UI Initialized", Font_7x10, White);
  ssd1306_UpdateScreen();
  HAL_Delay(1000);

  sm.current_page = UI_PAGE_DISPLAY_DEFAULT;
  sm.previous_page = UI_PAGE_DISPLAY_DEFAULT;
  sm.current_event = UI_EVENT_NONE;
  sm.force_redraw = 1;
  sm.is_initialized = 1;

  if (System_IsDebugging())
    Diagnostics_TransmitBlocking("UI Initialized\n");

  LED_Init();
}

void UI_Button_Process(void) {}

void UI_Keypad_Process(void) {}

void UI_LED_Process(void) {
  if (!sm.is_initialized)
    return;
  LED_Process();
}

void UI_OLED_Process(void) {
  if (!sm.is_initialized)
    return;

  uint8_t needs_redraw = sm.force_redraw;
  sm.force_redraw = 0;

  if (System_IsTimeUpdateFlagSet()) {
    needs_redraw = 1;
  }

  if (needs_redraw) {
    if (state_table[sm.current_page].draw_function != NULL) {
      state_table[sm.current_page].draw_function();
    }
  }
}

void UI_RequestPage(UIPage_t target_page) {
  if (!sm.is_initialized)
    return;

  perform_page_transition(target_page);
}

void UI_ProcessEvent(UIEvent_t event) {
  if (!sm.is_initialized || event == UI_EVENT_NONE)
    return;

  sm.current_event = event;

  for (size_t i = 0; i < NUM_TRANSITIONS; ++i) {
    if (transition_table[i].current_state == sm.current_page &&
        transition_table[i].trigger_event == event) {
      perform_page_transition(transition_table[i].next_state);
      break;
    }
  }
}

void UI_RequestRedraw(void) { sm.force_redraw = 1; }

// ==========================================
// PRIVATE HELPER FUNCTIONS
// ==========================================
void perform_page_transition(UIPage_t target_page) {
  if (target_page == UI_PAGE_RETURN) {
    target_page = sm.previous_page;
  }

  if (target_page == sm.current_page || target_page >= UI_PAGE_COUNT) {
    return;
  }

  // FIXED: Only save history if we are leaving a normal display page
  // (i.e., strictly before the warning block starts)
  if (sm.current_page < UI_PAGE_WARNING_UNSAFE_DRIVING) {
    sm.previous_page = sm.current_page;
  }

  sm.current_page = target_page;

  // Instead of drawing synchronously here, flag a redraw so
  // UI_OLED_Process handles it safely in the main loop cycle.
  sm.force_redraw = 1;
}

//============== Private Functions ==============
void draw_display_page_default(void) {
  display_time();
  display_string(display_buffer[1], sizeof(display_buffer[1]),
                 "== Measurements ==");
  display_string(display_buffer[2], sizeof(display_buffer[2]),
                 "Press -> display");
  draw_display_buffer();
}

void draw_display_page_1(void) {
  TelemetryData_t sensor_data = Telemetry_GetData();

  display_time();
  display_metric(display_buffer[1], sizeof(display_buffer[1]),
                 "Dist:", sensor_data.distance, "cm");
  display_metric(display_buffer[2], sizeof(display_buffer[2]),
                 "Temp:", sensor_data.temperature, "C");
  draw_display_buffer();
}

void draw_display_page_2(void) {
  TelemetryData_t sensor_data = Telemetry_GetData();

  display_time();
  display_metric(display_buffer[1], sizeof(display_buffer[1]),
                 "Accel:", sensor_data.acceleration.magnitude, "g");
  display_metric(display_buffer[2], sizeof(display_buffer[2]),
                 "Light:", sensor_data.light_level, "lux");
  draw_display_buffer();
}

void draw_display_page_3(void) {
  TelemetryData_t sensor_data = Telemetry_GetData();

  display_time();
  display_metric(display_buffer[1], sizeof(display_buffer[1]),
                 "Lat:", sensor_data.location.latitude, "");
  display_metric(display_buffer[2], sizeof(display_buffer[2]),
                 "Lon:", sensor_data.location.longitude, "");
}

void draw_display_page_4(void) {
  TelemetryData_t sensor_data = Telemetry_GetData();

  display_time();
  display_metric(display_buffer[1], sizeof(display_buffer[1]),
                 "Heading:", sensor_data.location.heading, "deg");
  display_metric(display_buffer[2], sizeof(display_buffer[2]),
                 "Speed:", sensor_data.location.speed, "km/h");
}

void draw_display_page_5(void) {
  AnalyticsData_t analytics = Analytics_GetData();

  display_string(display_buffer[0], sizeof(display_buffer[0]),
                 "Fuel Efficiency:");
  display_metric(display_buffer[1], sizeof(display_buffer[1]), "",
                 analytics.fuel_efficiency.km_per_liter, "km/L");
  display_metric(display_buffer[2], sizeof(display_buffer[2]), "",
                 analytics.fuel_efficiency.liters_per_100km, "L/100km");
}

void draw_data_entry_page_default(void) {
  display_time();
  display_string(display_buffer[1], sizeof(display_buffer[1]),
                 "=== Data Entry ===");
  display_string(display_buffer[2], sizeof(display_buffer[2]),
                 "Press -> display");
  draw_display_buffer();
}

void draw_data_entry_page_1(void) {
  TelemetryData_t sensor_data = Telemetry_GetData();

  uint8_t editing = System_IsEditing();

  display_string(display_buffer[0], sizeof(display_buffer[0]),
                 "Enter fuel liters");
  display_metric(display_buffer[1], sizeof(display_buffer[1]),
                 "Current:", sensor_data.fuel_data.fuel_level, "L");

  (editing) ? display_string(display_buffer[2], sizeof(display_buffer[2]),
                             "Press S3 to change")
            : display_string(display_buffer[2], sizeof(display_buffer[2]),
                             "Press S3 to accept");
}

void draw_data_entry_page_2(void) {
  AnalyticsData_t analytics = Analytics_GetData();
  uint8_t editing = System_IsEditing();

  display_string(display_buffer[0], sizeof(display_buffer[0]),
                 "Enter odometer km");
  display_metric(display_buffer[1], sizeof(display_buffer[1]),
                 "Current:", analytics.fuel_data.odometer, "km");

  (editing) ? display_string(display_buffer[2], sizeof(display_buffer[2]),
                             "Press S3 to change")
            : display_string(display_buffer[2], sizeof(display_buffer[2]),
                             "Press S3 to accept");
}

void draw_data_entry_page_3(void) {
  uint8_t logging_enabled = System_IsLoggingEnabled();

  display_string(display_buffer[0], sizeof(display_buffer[0]),
                 "Log data (Y/N)");
  display_string(display_buffer[1], sizeof(display_buffer[1]),
                 "‘*’ = Y / ‘#’ = N");
  (logging_enabled)
      ? display_string(display_buffer[2], sizeof(display_buffer[2]),
                       "Logging: Enabled")
      : display_string(display_buffer[2], sizeof(display_buffer[2]),
                       "Logging: Disabled");
}

void draw_diagnostics_page_default(void) {
  display_time();
  display_string(display_buffer[1], sizeof(display_buffer[1]),
                 "== Diagnostics ===");
  display_string(display_buffer[2], sizeof(display_buffer[2]),
                 "Press -> display");
  draw_display_buffer();
}

void draw_diagnostics_page_1(void) {
  TelemetryStatus_t status = Telemetry_GetStatus();
  display_time();
  display_string(display_buffer[1], sizeof(display_buffer[1]),
                 "-- Diagnostics ---");
  (status.sd_card_status)
      ? display_string(display_buffer[2], sizeof(display_buffer[2]),
                       "SD Card: OK")
      : display_string(display_buffer[2], sizeof(display_buffer[2]),
                       "SD Card: Not OK");
}

void draw_diagnostics_page_2(void) {
  TelemetryStatus_t status = Telemetry_GetStatus();

  display_time();
  display_string(display_buffer[1], sizeof(display_buffer[1]),
                 "-- Diagnostics ---");
  (status.mpu6050_status)
      ? display_string(display_buffer[2], sizeof(display_buffer[2]),
                       "MPU6050: OK")
      : display_string(display_buffer[2], sizeof(display_buffer[2]),
                       "MPU6050: Not OK");
}

void draw_diagnostics_page_3(void) {
  TelemetryStatus_t status = Telemetry_GetStatus();

  display_time();
  display_string(display_buffer[1], sizeof(display_buffer[1]),
                 "-- Diagnostics ---");
  (status.gps_status)
      ? display_string(display_buffer[2], sizeof(display_buffer[2]), "GPS: OK")
      : display_string(display_buffer[2], sizeof(display_buffer[2]),
                       "GPS: Not OK");
}

void draw_diagnostics_page_4(void) {
  uint8_t logging_enabled = System_IsLoggingEnabled();
  display_time();
  display_string(display_buffer[1], sizeof(display_buffer[1]),
                 "-- Diagnostics ---");
  (logging_enabled)
      ? display_string(display_buffer[2], sizeof(display_buffer[2]),
                       "Logging: Enabled")
      : display_string(display_buffer[2], sizeof(display_buffer[2]),
                       "Logging: Disabled");
}

void draw_warning_unsafe_driving(void) {
  TelemetryData_t sensor_data = Telemetry_GetData();

  display_string(display_buffer[0], sizeof(display_buffer[0]),
                 "===== WARNING ====");
  display_string(display_buffer[1], sizeof(display_buffer[1]),
                 "--Unsafe Driving--");
  display_metric(display_buffer[2], sizeof(display_buffer[2]),
                 "Accel:", sensor_data.acceleration.magnitude, "g");
}

void draw_warning_impact_detected(void) {
  TelemetryData_t sensor_data = Telemetry_GetData();

  display_string(display_buffer[0], sizeof(display_buffer[0]),
                 "===== WARNING ====");
  display_string(display_buffer[1], sizeof(display_buffer[1]),
                 "-Impact Detected--");
  display_metric(display_buffer[2], sizeof(display_buffer[2]),
                 "Accel:", sensor_data.acceleration.magnitude, "g");
}

void draw_warning_low_light(void) {
  TelemetryData_t sensor_data = Telemetry_GetData();

  display_string(display_buffer[0], sizeof(display_buffer[0]),
                 "===== WARNING ====");
  display_string(display_buffer[1], sizeof(display_buffer[1]),
                 "----Low Light-----");
  display_metric(display_buffer[2], sizeof(display_buffer[2]),
                 "Light:", sensor_data.light_level, "lux");
}

void draw_warning_proximity(void) {
  TelemetryData_t sensor_data = Telemetry_GetData();

  display_string(display_buffer[0], sizeof(display_buffer[0]),
                 "===== WARNING ====");
  display_string(display_buffer[1], sizeof(display_buffer[1]),
                 "----Proximity-----");
  display_metric(display_buffer[2], sizeof(display_buffer[2]),
                 "Dist:", sensor_data.distance, "cm");
}

void draw_warning_high_temperature(void) {
  TelemetryData_t sensor_data = Telemetry_GetData();

  display_string(display_buffer[0], sizeof(display_buffer[0]),
                 "===== WARNING ====");
  display_string(display_buffer[1], sizeof(display_buffer[1]),
                 "-High Temperature-");
  display_metric(display_buffer[2], sizeof(display_buffer[2]),
                 "Temp:", sensor_data.temperature, "C");
}

//========== Helper functions ==========
void display_metric(char *display, size_t max_len, const char *left,
                    float number, const char *unit) {
  char left_tmp[20];
  char right_tmp[20];

  strcpy(left_tmp, left);
  snprintf(right_tmp, sizeof(right_tmp), "%.1f %s", number, unit);

  int padding = 18 - strlen(left_tmp) - strlen(right_tmp);

  // Use the explicit max_len parameter instead of sizeof(display)
  snprintf(display, max_len, "%s%*s%s", left_tmp, padding, "", right_tmp);
}

void display_string(char *display, size_t max_len, const char *str) {
  snprintf(display, max_len, "%s", str);
}

void display_time() {
  RTC_TimeTypeDef time;
  RTC_DateTypeDef date;
  HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
  snprintf(display_buffer[0], sizeof(display_buffer[0]),
           "=20%02d/%02d/%02d %02d:%02d=", date.Year, date.Month, date.Date,
           time.Hours, time.Minutes);
}

void draw_display_buffer(void) {
  ssd1306_Fill(Black);
  for (int i = 0; i < 3; i++) {
    ssd1306_SetCursor(0, i * 10);
    ssd1306_WriteString(display_buffer[i], Font_7x10, White);
  }
  ssd1306_UpdateScreen();
}
