#include "DispenserMotorDriver.h"

namespace tipuino {

  DispenserMotorDriver::DispenserMotorDriver(
    Hal* hal,
    const pin_t enablePinArg,
    const pin_t stepPinArg,
    const pin_t dirPinArg,
    const pin_t uartRx,
    const pin_t uartTx,
    const pin_t homePinArg,
    const PinValue homeDirection
  ) : StepperMotorDriver(hal, enablePinArg, stepPinArg, dirPinArg, uartRx, uartTx)
    , HomingStepperMotorMixin()
    , homePin(homePinArg)
    , homeDirection(homeDirection)
    , homingInterface(this, homePin, homeDirection)
    , 
  { }

  void DispenserMotorDriver::setup() {
    homePin.setup();
    StepperMotorDriver::setup();
  }

  HomingStepperMotorMixin::Interface& interface() {
    return homingInterface;
  }
}
