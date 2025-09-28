#include "HomiingStepperMotorDriver.h"

namespace tipuino {

  HomingStepperMotorDriver::HomingStepperMotorDriver(
    pin_t enablePin,
    pin_t stepPin,
    pin_t dirPin,
    pin_t uartRx,
    pin_t uartTx,
    pin_t homePin
  ) : StepperMotorDriver(enablePin, stepPin, dirPin, uartRx, uartTx)
    , homePin(homePin)
  {}

  bool HomingStepperMotorDriver::isHome() const {
    return digitalRead(homePin) == LOW;
  }

}
