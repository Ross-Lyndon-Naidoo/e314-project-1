#ifndef INC_UTILS_H
#define INC_UTILS_H

#include <stdint.h>

typedef struct {
  int whole;
  int fraction;
} SplitFloat_t;

SplitFloat_t utils_split_float(float value, uint8_t decimal_places);

#endif