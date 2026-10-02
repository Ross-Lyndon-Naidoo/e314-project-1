/*
 * diagnostics.h
 *
 *  Created on: 29 Sept 2026
 *      Author: Rosso
 */

#ifndef INC_DIAGNOSTICS_H_
#define INC_DIAGNOSTICS_H_

#include "system_types.h"

typedef struct {
  uint8_t tx_flag;
  uint8_t rx_flag;
  char tx_buffer[64];
  char rx_buffer[64];
  uint16_t rx_index;
} DiagnosticsData_t;

/**
 * @brief Initialize the diagnostics module.
 */
void Diagnostics_Init(void);

/**
 * @brief Check if there are any new commands and process them
 */
void Diagnostics_Process(void);
void Diagnostics_TransmitData(const char *data);
void Diagnostics_TransmitBlocking(const char *data);
#endif /* INC_DIAGNOSTICS_H_ */
