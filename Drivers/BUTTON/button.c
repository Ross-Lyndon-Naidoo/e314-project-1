/*
 * button.c
 *
 *  Created on: 30 Sept 2026
 *      Author: Rosso
 */

#include "button.h"
#include "main.h"
#include "ui.h"
#include <stdint.h>

typedef struct {
  uint16_t gpio_pin;      // e.g., GPIO_PIN_0
  UIEvent_t mapped_event; // The event this button fires
  uint32_t last_tick;     // For software debouncing
  volatile uint8_t triggered;
} Button_t;

static Button_t buttons[5] = {{BTN_S5_Pin, UI_EVENT_NAV_NEXT, 0, 0},
                              {BTN_S1_Pin, UI_EVENT_NAV_PREV, 0, 0},
                              {BTN_S4_Pin, UI_EVENT_NAV_ENTER, 0, 0},
                              {BTN_S2_Pin, UI_EVENT_NAV_BACK, 0, 0},
                              {BTN_S3_Pin, UI_EVENT_NAV_SELECT, 0, 0},
                              {0, UI_EVENT_NONE, 0, 0}

};
