#include "hal/Atmega2560.h"

namespace tipuino::hal {

  void Atmega2560::writePin(const pin_t pin, const pin_value_t value) const {
  }

  pin_value_t Atmega2560::readPin(const pin_t pin) const {
    return 0;
  }
}
