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

/**
 * @brief Get the current analytics data.
 * @return The current analytics data.
 */
AnalyticsData_t Analytics_GetAnalyticsData(void);

/**
 * @brief Get the current warning flags.
 * @return The current warning flags.
 */
warning_flags_t Analytics_GetWarnings(void);
/**
 * @brief Set the warning flags.
 * @param flags The warning flags to set.
 */
void Analytics_SetWarnings(warning_flags_t flags);

/**
 * @brief Set the fuel efficiency.
 * @param efficiency The fuel efficiency to set.
 */
void Analytics_SetFuelEfficiency(fuel_efficiency_t efficiency);

#endif /* INC_ANALYTICS_H_ */
