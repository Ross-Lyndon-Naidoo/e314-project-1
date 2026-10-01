/*
 * system_types.h
 *
 *  Created on: 30 Sept 2026
 *      Author: Rosso
 */

#ifndef INC_SYSTEM_TYPES_H_
#define INC_SYSTEM_TYPES_H_

#include <stdint.h>
#include "main.h"

typedef struct {
    float x;
    float y;
    float z;
    float magnitude;
} acceleration_data_t;

typedef struct{
    double latitude;
    double longitude;
    uint16_t heading;
    float speed;
} location_t;

typedef struct{
    float fuel_level;
    float odometer;
} fuel_data_t;

typedef struct{
    float km_per_liter;
    float liters_per_100km;
} fuel_efficiency_t;

typedef struct{
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;
} timestamp_t;

typedef struct{
    uint8_t unsafe_driving;
    uint8_t impact_detected;
    uint8_t low_light;
    uint8_t proximity;
    uint8_t high_temperature;
} warning_flags_t;






#endif /* INC_SYSTEM_TYPES_H_ */
