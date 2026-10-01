/*
 * led.c
 *
 *  Created on: 30 Sept 2026
 *      Author: Rosso
 */
#include "led.h"
#include "main.h"
#include "system.h"

typedef struct {
  GPIO_TypeDef *gpio_port;
  uint16_t gpio_pin;
  LED_State_t state;
} LED_t;

static LED_t leds[LED_COUNT] = {
    {LED_D2_GPIO_Port, LED_D2_Pin, LED_STATE_OFF},
    {LED_D3_GPIO_Port, LED_D3_Pin, LED_STATE_OFF},
    {LED_D4_GPIO_Port, LED_D4_Pin, LED_STATE_OFF},
    {LED_D5_GPIO_Port, LED_D5_Pin, LED_STATE_OFF},
    {LED_DEBUG_GPIO_Port, LED_DEBUG_Pin, LED_STATE_OFF}};

void LED_Init(void) {
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i].state = LED_STATE_OFF;
    HAL_GPIO_WritePin(leds[i].gpio_port, leds[i].gpio_pin, GPIO_PIN_RESET);
  }

  (System_IsDebugging()) ? LED_SetState(LED_DEBUG, LED_STATE_ON)
                         : LED_SetState(LED_DEBUG, LED_STATE_OFF);
}

void LED_Process(void) {

  GPIO_PinState blink_state =
      ((HAL_GetTick() % 1000) < 500) ? GPIO_PIN_SET : GPIO_PIN_RESET;

  for (int i = 0; i < LED_COUNT; i++) {
    GPIO_PinState pin_target = GPIO_PIN_RESET;

    switch (leds[i].state) {
    case LED_STATE_OFF:
      pin_target = GPIO_PIN_RESET;
      break;
    case LED_STATE_ON:
      pin_target = GPIO_PIN_SET;
      break;
    case LED_STATE_BLINK:
      pin_target = blink_state;
      break;
    }

    HAL_GPIO_WritePin(leds[i].gpio_port, leds[i].gpio_pin, pin_target);
  }
}

void LED_SetState(LED_ID_t id, LED_State_t state) {
  if (id < LED_COUNT) {
    leds[id].state = state;
  }
}
