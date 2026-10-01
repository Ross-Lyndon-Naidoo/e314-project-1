/*
 * analytics.h
 *
 *  Created on: 29 Sept 2026
 *      Author: Rosso
 */

#ifndef INC_ANALYTICS_H_
#define INC_ANALYTICS_H_
#include "rtc.h"
#include "system_types.h"
//============== Public Types ==============
typedef struct {
  fuel_efficiency_t fuel_efficiency;
  fuel_data_t fuel_data;
  uint8_t is_editing;
} AnalyticsData_t;

//============== Public Function Prototypes ==============
/**
 * @brief Initialize the analytics module.
 */
void Analytics_Init(void);

/**
 * @brief Process sensor data and update analytics.
 */
void Analytics_Process(void);

//============= Getters and Setters ==============


/**
 * @brief Set the editing state to true
 *
 */
void Analytics_SetEditing(void);

/**
 * @brief Clear the editing state
 *
 */
void Analytics_ClearEditing(void);
/**
 * @brief Check if the editing state is true
 * @return 1 if the editing state is true, 0 otherwise.
 */
uint8_t Analytics_IsEditing(void);

warning_flags_t Analytics_GetWarningFlags(void);
void Analytics_SetWarningFlags(warning_flags_t flags);
void Analytics_ClearWarningFlags(void);

fuel_efficiency_t Analytics_GetFuelEfficiency(void);
void Analytics_SetFuelEfficiency(fuel_efficiency_t efficiency);
void Analytics_CalculateFuelEfficiency(void);

AnalyticsData_t Analytics_GetAnalyticsData(void);

#endif /* INC_ANALYTICS_H_ */
