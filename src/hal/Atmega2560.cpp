#include "hal/Atmega2560.h"

#include "platform.h"

namespace tipuino {

  Hal* Hal::init() {
    return new hal::Atmega2560();
  }
}

namespace tipuino::hal {

  void Atmega2560::writePin(const pin_t pin, const PinValue value) const {
    digitalWrite(pin, static_cast<uint8_t>(value));
  }

  PinValue Atmega2560::readPin(const pin_t pin) const {
    return static_cast<PinValue>(digitalRead(pin));
  }

  void Atmega2560::setPinMode(const pin_t pin, const PinMode mode) const {
    pinMode(pin, static_cast<uint8_t>(mode));
  }
}
