#include "Hal.h"

namespace tipuino {

  // UsePin
  UsePin::UsePin(Pin& targetPin) : pin(targetPin) { pin.invValue(); }

  UsePin::~UsePin() { pin.invValue(); }

  //Pin
  Pin::Pin(const Hal* hal, const pin_t pinNumber, pin_value_t initialValue)
    : hal(hal), pin(pinNumber), value(value) { hal->writePin(pin, initialValue); }

  Pin::Pin(const Hal* hal, const pin_t pinNumber)
    : hal(hal), pin(pinNumber), value(hal->readPin(pin)) {}

  UsePin Pin::use() { return UsePin(hal, pin, value); }

  void Pin::write(const pin_value_t value) { value = newValue; hal->writePin(pin, value); }

  void Pin::sync() { value = hal->readPin(pin); }

}
