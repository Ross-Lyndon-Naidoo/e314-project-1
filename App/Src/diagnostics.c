/*
 * diagnostics.c
 *
 *  Created on: 29 Sept 2026
 *      Author: Rosso
 */

#include "diagnostics.h"
#include "main.h"
#include "system.h"
#include "telemetry.h"
#include "ui.h"
#include "usart.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  const char *prefix;
  void (*execute)(const char *payload);
} UART_Command_t;

static char tx_buffer[256];
static volatile uint8_t tx_busy = 0;

static char rx_buffer[128];
static char process_buffer[128];
static volatile uint8_t rx_data_ready = 0;

extern UART_HandleTypeDef huart2;

//============== 1. Command Execution Functions ==============
static void cmd_stat(const char *payload);
static void cmd_log(const char *payload);
static void cmd_setwarn(const char *payload);
static void cmd_sfd(const char *payload);
static void cmd_setrtc(const char *payload);
static void cmd_setpage(const char *payload);
static void cmd_s(const char *payload);
static void format_stat_msg(char *out_buffer, size_t out_size, const char *msg,
                            float value, const char *format);

//============== 2. The Command Table ==============
static const UART_Command_t command_table[] = {
    {"@Stat&", cmd_stat},                // Fixed
    {"@Log&", cmd_log},                  // Point to a cmd_log handler
    {"@SetWarn ", cmd_setwarn},          // Parameterized
    {"@SFD ", cmd_sfd},                  // Parameterized
    {"@SetRTC ", cmd_setrtc},            // Parameterized
    {"@SetPage ", cmd_setpage},          // Parameterized
    {.prefix = "@S ", .execute = cmd_s}, // Parameterized
};

#define NUM_COMMANDS (sizeof(command_table) / sizeof(command_table[0]))

//============== Public Functions ==============
void Diagnostics_Init(void) {
  HAL_UARTEx_ReceiveToIdle_IT(&huart2, (uint8_t *)rx_buffer, sizeof(rx_buffer));

  if (System_IsDebugging())
    Diagnostics_TransmitBlocking("Diagnostics Initialized\n");
}

void Diagnostics_Process(void) {
  if (rx_data_ready) {
    rx_data_ready = 0;
    uint8_t command_found = 0;

    // Iterate through table and match prefixes using the SAFE process_buffer
    for (int i = 0; i < NUM_COMMANDS; i++) {
      int prefix_len = strlen(command_table[i].prefix);

      if (strncmp(process_buffer, command_table[i].prefix, prefix_len) == 0) {

        // Pass the safe buffer to the handler
        command_table[i].execute(process_buffer);
        command_found = 1;
        break;
      }
    }

    if (!command_found) {
      Diagnostics_TransmitData("Error: Unknown command\n");
    }
  }
}

void Diagnostics_TransmitData(const char *data) {
  // Check the specific TX state so background RX (like GPS) doesn't block this
  if (!tx_busy) {
    strncpy(tx_buffer, data, sizeof(tx_buffer) - 1);
    tx_buffer[sizeof(tx_buffer) - 1] = '\0';

    tx_busy = 1; // Lock the buffer

    // Attempt to transmit. If the HAL rejects it, unlock the buffer.
    if (HAL_UART_Transmit_IT(&huart2, (uint8_t *)tx_buffer,
                             strlen(tx_buffer)) != HAL_OK) {
      tx_busy = 0;
    }
  }
}

void Diagnostics_TransmitBlocking(const char *data) {
  while (tx_busy) {
  }
  HAL_UART_Transmit(&huart2, (uint8_t *)data, strlen(data), 1000);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
  if (huart->Instance == USART2) {
    uint16_t copy_size =
        (Size < sizeof(process_buffer)) ? Size : (sizeof(process_buffer) - 1);
    memcpy(process_buffer, rx_buffer, copy_size);

    // 2. Safely null-terminate it so string functions don't crash
    process_buffer[copy_size] = '\0';

    rx_data_ready = 1;

    // 3. RE-ARM IMMEDIATELY! The hardware starts listening again instantly.
    HAL_UARTEx_ReceiveToIdle_IT(&huart2, (uint8_t *)rx_buffer,
                                sizeof(rx_buffer));
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART2) {
    tx_busy = 0; // Transmission complete! Unlock the buffer.
  }
}

static void cmd_stat(const char *payload) {
  (void)payload;

  const TelemetryData_t telemetry = Telemetry_GetSensorData();
  const timestamp_t time = System_GetCurrentTime();

  char response[256];
  size_t used = 0U;

  used += (size_t)snprintf(response + used, sizeof(response) - used,
                           "@20%02u/%02u/%02u %02u:%02u:%02u\n", time.date.Year,
                           time.date.Month, time.date.Date, time.time.Hours,
                           time.time.Minutes, time.time.Seconds);

  format_stat_msg(response + used, sizeof(response) - used,
                  "Distance:", telemetry.distance, "%03.1f cm");
  used = strlen(response);

  format_stat_msg(response + used, sizeof(response) - used,
                  "Temperature:", telemetry.temperature, "%03.1f C");
  used = strlen(response);

  format_stat_msg(response + used, sizeof(response) - used,
                  "Light:", (float)telemetry.light_level, "%04.0f lux");
  used = strlen(response);

  format_stat_msg(response + used, sizeof(response) - used,
                  "X accel:", telemetry.acceleration.x, "%03.1f g");
  used = strlen(response);

  format_stat_msg(response + used, sizeof(response) - used,
                  "Y accel:", telemetry.acceleration.y, "%03.1f g");
  used = strlen(response);

  format_stat_msg(response + used, sizeof(response) - used,
                  "Z accel:", telemetry.acceleration.z, "%03.1f g");

  Diagnostics_TransmitData(response);
}
static void cmd_log(const char *payload) {
  (void)payload;

  char response[64];

  const uint8_t enabled = !System_IsLoggingEnabled();
  System_SetLoggingEnabled(enabled);

  snprintf(response, sizeof(response), "Logging %s\n",
           enabled ? "enabled" : "disabled");
  Diagnostics_TransmitData(response);
}

static void cmd_setwarn(const char *payload) {
  char condition;
  int state;

  // sscanf parses the payload. It returns the number of variables successfully
  // read. We expect exactly 2 variables to match the pattern: "@SetWarn C=Y&"
  if (sscanf(payload, "@SetWarn %c=%d&", &condition, &state) == 2) {
    char response[64];
    snprintf(response, sizeof(response), "Warning '%c' set to %d\n", condition,
             state);
    Diagnostics_TransmitData(response);
  } else {
    Diagnostics_TransmitData("Error: Invalid SetWarn format\n");
  }
}

// Variables: Fuel (float) and Kilometers (float)
static void cmd_sfd(const char *payload) {
  float fuel, km;

  // Matches pattern: "@SFD xxx.x;zzz.z&"
  if (sscanf(payload, "@SFD %f;%f&", &fuel, &km) == 2) {
    char response[64];
    snprintf(response, sizeof(response), "Logged %.1f L at %.1f km\n", fuel,
             km);
    Diagnostics_TransmitData(response);
  } else {
    Diagnostics_TransmitData("Error: Invalid SFD format\n");
  }
}

// Variables: Date and Time values
static void cmd_setrtc(const char *payload) {
  int year, month, day, hour, min, sec;

  // Matches pattern: "@SetRTC YYYY/MM/DD;HH:MM:SS&"
  if (sscanf(payload, "@SetRTC %d/%d/%d;%d:%d:%d&", &year, &month, &day, &hour,
             &min, &sec) == 6) {
    // Here you would call your HAL_RTC_SetTime and HAL_RTC_SetDate functions
    char response[64];
    snprintf(response, sizeof(response), "RTC updated to %04d/%02d/%02d\n",
             year, month, day);
    Diagnostics_TransmitData(response);
  } else {
    Diagnostics_TransmitData("Error: Invalid SetRTC format\n");
  }
}

static void cmd_setpage(const char *payload) {
  int page_number;

  // Matches pattern: "@SetPage N&"
  if (sscanf(payload, "@SetPage %d&", &page_number) == 1) {
    if (page_number >= 0 && page_number < UI_PAGE_COUNT) {
      UI_RequestPage((UIPage_t)page_number);
      Diagnostics_TransmitData("Page change requested\n");
    } else {
      Diagnostics_TransmitData("Error: Invalid page number\n");
    }
  } else {
    Diagnostics_TransmitData("Error: Invalid SetPage format\n");
  }
}

static void cmd_s(const char *payload) {
  int button_id;

  // 1. Parse the integer from the payload string (e.g., "@S 1&")
  if (sscanf(payload, "@S %d&", &button_id) == 1) {
    char response[64];
    UIEvent_t target_event = UI_EVENT_NONE;

    // 2. Safely map the integer to your defined UIEvent_t enum
    switch (button_id) {
    case 1:
      target_event = UI_EVENT_NAV_PREV;
      break;
    case 2:
      target_event = UI_EVENT_NAV_BACK;
      break;
    case 3:
      target_event = UI_EVENT_NAV_SELECT;
      break;
    case 4:
      target_event = UI_EVENT_NAV_ENTER;
      break;
    case 5:
      target_event = UI_EVENT_NAV_NEXT;
      break;
    default:
      Diagnostics_TransmitData("Error: Invalid button ID\n");
      return; // Exit early if the number doesn't map to a real event
    }

    // 3. Feed the emulated event straight into the UI state machine
    UI_ProcessEvent(target_event);

    // 4. Send success response over UART
    snprintf(response, sizeof(response), "Button %d emulated\n", button_id);
    Diagnostics_TransmitData(response);

  } else {
    Diagnostics_TransmitData("Error: Invalid S format\n");
  }
}

static void format_stat_msg(char *out_buffer, size_t out_size, const char *msg,
                            float value, const char *format) {
  char val_buffer[16];

  (void)snprintf(val_buffer, sizeof(val_buffer), format, value);

  const size_t value_length = strlen(val_buffer);
  const size_t message_width = value_length < 20U ? 20U - value_length : 0U;

  int message_length = (int)strlen(msg);
  if ((size_t)message_length > message_width) {
    message_length = (int)message_width;
  }

  const int value_width = 20 - message_length;

  (void)snprintf(out_buffer, out_size, "%.*s%*s\n", message_length, msg,
                 value_width, val_buffer);
}
