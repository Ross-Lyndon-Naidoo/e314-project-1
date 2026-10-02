/**
 *  @file telemetry.h
 *  @brief Telemetry data structure and function prototypes.
 *  @details This file contains the telemetry service
 *  @author Rosso
 *  @date 29 Sept 2026
 */

#ifndef INC_TELEMETRY_H_
#define INC_TELEMETRY_H_

#include "system_types.h"
#include <stdint.h>

typedef struct {
  float distance;
  float temperature;
  uint16_t light_level;
  acceleration_data_t acceleration;
  location_t location;
  fuel_data_t fuel_data;
} TelemetryData_t;

typedef struct {
  uint8_t sd_card_status;
  uint8_t mpu6050_status;
  uint8_t gps_status;
} TelemetryStatus_t;

//============== Public Function Prototypes ==============
/**
 * @brief Initialize the all sensors.
 */
void Telemetry_Init(void);

/**
 * @brief Process raw sensor data and update the current data.
 */
void Telemetry_Process(void);

/**
 * @brief Get the current sensor data.
 * @return A TelemetryData_t structure containing the current sensor data.
 */
TelemetryData_t Telemetry_GetData(void);

TelemetryStatus_t Telemetry_GetStatus(void);
#endif /* INC_TELEMETRY_H_ */
