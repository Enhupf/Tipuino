#include "platform.h"

namespace tipuino {

  PinValue inv(const PinValue value) {
    return value == PinValue::PinValueLow ?
      PinValue::PinValueHigh :
      PinValue::PinValueLow;
  }
}
