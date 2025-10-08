#include "hal/Atmega2560.h"

#include "platform.h"

namespace tipuino::hal {

  void Atmega2560::writePin(const pin_t pin, const PinValue value) const {}

  PinValue Atmega2560::readPin(const pin_t pin) const {
    return PinValue::PinValueLow;
  }

  void Atmega2560::pinMode(const pin_t pin, const PinMode mode) const {}
}
