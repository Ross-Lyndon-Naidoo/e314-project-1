/*
 * logging.h
 *
 *  Created on: 29 Sept 2026
 *      Author: Rosso
 */

#ifndef INC_LOGGING_H_
#define INC_LOGGING_H_

#include "system_types.h"


void Logging_Process(void);
void Logging_SetLogUpdateFlag(void);
void Logging_ClearLogUpdateFlag(void);
uint8_t Logging_IsLogUpdateFlagSet(void);

void Logging_EnableLogging(void);
void Logging_DisableLogging(void);
uint8_t Logging_IsLoggingEnabled(void);

uint8_t Logging_SDCardStatus(void);
uint8_t Logging_MPU6050Status(void);
uint8_t Logging_GPSStatus(void);


#endif /* INC_LOGGING_H_ */
