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
static const char *format_stat_msg(const char *msg, float value,
                                   const char *format);

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
  HAL_UARTEx_ReceiveToIdle_IT(&huart2, rx_buffer, sizeof(rx_buffer));

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
    HAL_UARTEx_ReceiveToIdle_IT(&huart2, rx_buffer, sizeof(rx_buffer));
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART2) {
    tx_busy = 0; // Transmission complete! Unlock the buffer.
  }
}

static void cmd_stat(const char *payload) {
  (void)payload;

  const System_Context_t *system = System_GetContext();
  const timestamp_t time = System_GetCurrentTime();
  char response[256];

  snprintf(
      response, sizeof(response), "@20%02d/%02d/%02d %02d:%02d:%02d \n%s\n",
      time.date.Year, time.date.Month, time.date.Date, time.time.Hours,
      time.time.Minutes, time.time.Seconds,
      format_stat_msg("Distance:", system->telemetry.distance, "%03.1f cm"));

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

#include <stdio.h>
#include <string.h>

static const char *format_stat_msg(const char *msg, float value,
                                   const char *format) {
  // 20 visible characters + 1 '\n' + 1 '\0' terminator = 22 bytes
  static char out_buffer[22];
  char val_buffer[16];

  // 1. Format the value first to know its exact string length
  snprintf(val_buffer, sizeof(val_buffer), format, value);
  int val_len = strlen(val_buffer);

  // 2. Calculate the maximum space left for the message
  int max_msg_len = 20 - val_len;
  if (max_msg_len < 0)
    max_msg_len = 0;

  // 3. Truncate the message if it's too long to prevent throwing off the
  // 21-char limit
  int msg_len = strlen(msg);
  if (msg_len > max_msg_len) {
    msg_len = max_msg_len;
  }

  // 4. The value takes up all remaining space (this forces right-alignment)
  int val_width = 20 - msg_len;

  // 5. Construct the final string
  // %.*s prints the left-aligned message
  // %*s  prints the value right-aligned within the val_width space
  snprintf(out_buffer, sizeof(out_buffer), "%.*s%*s\n", msg_len, msg, val_width,
           val_buffer);

  return out_buffer;
}
