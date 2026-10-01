/*
 * leds.h
 *
 *  Created on: 30 Sept 2026
 *      Author: Rosso
 */

#ifndef INC_LED_H_
#define INC_LED_H_

typedef enum {
  LED_D2 = 0,
  LED_D3,
  LED_D4,
  LED_D5,
  LED_DEBUG,
  LED_COUNT
} LED_ID_t;

typedef enum { LED_STATE_OFF = 0, LED_STATE_ON, LED_STATE_BLINK } LED_State_t;

/**
 * @brief Initialize the LED module.
 */
void LED_Init(void);

/**
 * @brief Process the LED module.
 */
void LED_Process(void);

/**
 * @brief Set the state of a specific LED.
 * @param id The ID of the LED to set.
 * @param state The state to set the LED to.
 */
void LED_SetState(LED_ID_t id, LED_State_t state);

#endif /* INC_LED_H_ */
