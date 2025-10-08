#include "Hal.h"

namespace tipuino {

  // UsePin
  UsePin::UsePin(Pin* targetPin) : pin(targetPin) { pin->invValue(); }

  UsePin::~UsePin() { pin->invValue(); }

  //Pin
  Pin::Pin(const Hal* hal, const pin_t pinNumber, const PinValue initialValue)
    : hal(hal), pin(pinNumber), value(initialValue) { hal->writePin(pin, initialValue); }

  Pin::Pin(const Hal* hal, const pin_t pinNumber)
    : hal(hal), pin(pinNumber), value(hal->readPin(pin)) {}

  UsePin Pin::use() { return UsePin(this); }

  void Pin::write(const PinValue newValue) { value = newValue; hal->writePin(pin, value); }

  PinValue Pin::sync() { value = hal->readPin(pin); return value; }

  Hal::~Hal() {}

}
