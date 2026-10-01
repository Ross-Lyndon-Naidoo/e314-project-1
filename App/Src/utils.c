#include "utils.h"
#include <math.h>

SplitFloat_t utils_split_float(float value, uint8_t decimal_places) {
  SplitFloat_t result = {0};

  if (decimal_places > 6U) {
    decimal_places = 6U;
  }

  int scale = 1;
  for (uint8_t i = 0U; i < decimal_places; ++i) {
    scale *= 10;
  }

  result.whole = (int)value;

  const float remainder = value - (float)result.whole;
  result.fraction = (int)roundf(fabsf(remainder) * (float)scale);

  if (result.fraction >= scale) {
    result.fraction = 0;

    if (value >= 0.0f) {
      result.whole++;
    } else {
      result.whole--;
    }
  }

  return result;
}