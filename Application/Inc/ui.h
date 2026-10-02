/*
 * ui.h
 *
 *  Created on: 29 Sept 2026
 *      Author: Rosso
 */

#ifndef INC_UI_H_
#define INC_UI_H_

//========== Types ==========
typedef enum {
    UI_PAGE_DISPLAY_DEFAULT = 0,
    UI_PAGE_DATA_ENTRY_DEFAULT,
    UI_PAGE_DIAGNOSTICS_DEFAULT,
    UI_PAGE_DISPLAY_1,
    UI_PAGE_DISPLAY_2,
    UI_PAGE_DISPLAY_3,
    UI_PAGE_DISPLAY_4,
    UI_PAGE_DISPLAY_5,
    UI_PAGE_DATA_ENTRY_1,
    UI_PAGE_DATA_ENTRY_2,
    UI_PAGE_DATA_ENTRY_3,
    UI_PAGE_DIAGNOSTICS_1,
    UI_PAGE_DIAGNOSTICS_2,
    UI_PAGE_DIAGNOSTICS_3,
    UI_PAGE_DIAGNOSTICS_4,
    UI_PAGE_WARNING_UNSAFE_DRIVING,
    UI_PAGE_WARNING_IMPACT_DETECTED,
    UI_PAGE_WARNING_LOW_LIGHT,
    UI_PAGE_WARNING_PROXIMITY,
    UI_PAGE_WARNING_HIGH_TEMPERATURE,
    UI_PAGE_COUNT
} UIPage_t;

#define UI_PAGE_STAY   (UIPage_t)(UI_PAGE_COUNT + 0)
#define UI_PAGE_RETURN (UIPage_t)(UI_PAGE_COUNT + 1)

typedef enum {
    UI_EVENT_NONE = 0,
    UI_EVENT_ANY,
    UI_EVENT_NAV_PREV,
    UI_EVENT_NAV_BACK,
    UI_EVENT_NAV_SELECT,
    UI_EVENT_NAV_ENTER,
    UI_EVENT_NAV_NEXT,
    UI_EVENT_KEY_0,
    UI_EVENT_KEY_1,
    UI_EVENT_KEY_2,
    UI_EVENT_KEY_3,
    UI_EVENT_KEY_4,
    UI_EVENT_KEY_5,
    UI_EVENT_KEY_6,
    UI_EVENT_KEY_7,
    UI_EVENT_KEY_8,
    UI_EVENT_KEY_9,
    UI_EVENT_KEY_STAR,
    UI_EVENT_KEY_HASH,
    UI_EVENT_COUNT
} UIEvent_t;

typedef struct {
    UIPage_t current_state;
    UIEvent_t trigger_event;
    UIPage_t next_state;
} Transition_t;

//============== Public Function Prototypes ==============
/**
 * @brief Initialize the LEDs, OLED, Buttons, and the Keypad
 */
void UI_Init(void);

void UI_ProcessEvent(UIEvent_t event);

/**
 * @brief Process push button inputs
 */
void UI_Button_Process(void);

/**
 * @brief Process keypad inputs
 */
void UI_Keypad_Process(void);

/**
 * @brief Process and update the current page
 */
void UI_LED_Process(void);

/**
 * @brief Process and update the current page
 */
void UI_OLED_Process(void);

/**
 * @brief Process and update the current page
 * @param target_page The target page to change to
 */
void UI_RequestPage(UIPage_t target_page);

//========== Getters and Setters ==========
/**
 * @brief Request a redraw of the current page
 */
void UI_RequestRedraw(void);

#endif /* INC_UI_H_ */
