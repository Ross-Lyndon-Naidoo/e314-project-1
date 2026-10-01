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
void Logging_SetLoggingUpdate(void);
void Logging_ClearLoggingUpdate(void);
uint8_t Logging_IsLoggingUpdate(void);

#endif /* INC_LOGGING_H_ */
