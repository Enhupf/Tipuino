#include "DispenserMotorDriver.h"

#include "platform.h"

#define DISPENSER_CLEAR_STEPS 350

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
    , homePin(hal, homePinArg)
    , homeDirection(homeDirection)
    , homingInterface { this, homePin, homeDirection }
  { }

  void DispenserMotorDriver::setup() {
    homePin.setup();
    StepperMotorDriver::setup();
  }

  HomingStepperMotorMixin::Interface& DispenserMotorDriver::interface() {
    return homingInterface;
  }

  void DispenserMotorDriver::homeMotor() {
    HomingStepperMotorMixin::homeMotor();

    int count = 0;
    auto clearMotor = [this, &count] {

      if(count == 0 && homePin.sync() == PinValue::PinValueLow) {
        return true;
      }
      return count++ < DISPENSER_CLEAR_STEPS;
    };
    setDirection(inv(homeDirection));
    stepWhile(clearMotor, 0);
  }
}
