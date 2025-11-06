#include "Hal.h"

#include "platform.h"

namespace tipuino {

  // UsePin
  UsePin::UsePin(Pin* targetPin) : pin(targetPin) { pin->invValue(); }

  UsePin::~UsePin() { pin->invValue(); }

  //Pin
  Pin::Pin(const Hal* hal, const pin_t pinNumber, const PinValue initialValue)
  : hal(hal), pinMode(PinMode::PinModeOutput), pin(pinNumber), value(initialValue)  {
  }

  Pin::Pin(const Hal* hal, const pin_t pinNumber)
  : hal(hal), pinMode(PinMode::PinModeInputPullup), pin(pinNumber)
  {
  }

  void Pin::setup() {
    switch(pinMode) {
      case PinMode::PinModeOutput:
        hal->setPinMode(pin, PinMode::PinModeOutput);
        hal->writePin(pin, value);
        break;

      case PinMode::PinModeInputPullup:
        hal->setPinMode(pin, PinMode::PinModeInputPullup);
        value = hal->readPin(pin);
        break;

      default:
        // Throw an exception, this will set the system's
        // state to "Error"
        break;
    }
  }

  UsePin Pin::use() { return UsePin(this); }

  void Pin::write(const PinValue newValue) { value = newValue; hal->writePin(pin, value); }

  PinValue Pin::sync() { value = hal->readPin(pin); return value; }

  Hal::~Hal() {}

}
