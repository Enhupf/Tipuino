#ifndef PRELUDE_H
#define PRELUDE_H

#include <Arduino.h>

using pin_t = uint8_t;
using pin_value_t = uint8_t;

inline pin_value_t inv(const pin_value_t value) {
  return value == HIGH ? LOW : HIGH;
}

#endif
