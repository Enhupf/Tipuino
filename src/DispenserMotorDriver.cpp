#include "DispenserMotorDriver.h"

#include "platform.h"
#include "TipuinoError.h"

#define DISPENSER_CLEAR_STEPS 350

namespace tipuino {

  const PinValue dispenserHomeDirection = PinValue::PinValueHigh;

  DispenserMotorDriver::DispenserMotorDriver(
    Hal* hal,
    const pin_t enablePinArg,
    const pin_t stepPinArg,
    const pin_t dirPinArg,
    const pin_t uartRx,
    const pin_t uartTx,
    const pin_t homePinArg,
    const pin_t beamPinArg
  ) : StepperMotorDriver(hal, enablePinArg, stepPinArg, dirPinArg, uartRx, uartTx)
    , HomingStepperMotorMixin()
    , homePin(hal, homePinArg)
    , beamPin(hal, beamPinArg)
    , homingInterface { this, homePin, dispenserHomeDirection }
  { }

  void DispenserMotorDriver::setup() {
    homePin.setup();
    beamPin.setup();
    StepperMotorDriver::setup();

    homeMotor();
    addClearance();
  }

  HomingStepperMotorMixin::Interface& DispenserMotorDriver::interface() {
    return homingInterface;
  }

  void DispenserMotorDriver::addClearance() {

    /*
    if(!isHome()) {
      setError(
        TipuinoError::InvalidOperationError,
        "Attempted to add clearance without homing the stepepr motor."
      );
      return;
    }
    */

    int count = 0;
    auto clearMotor = [this, &count] {

      if(count == 0 && beamPin.sync() == PinValue::PinValueLow) {
        return true;
      }
      return count++ < DISPENSER_CLEAR_STEPS;
    };
    setDirection(inv(dispenserHomeDirection));
    stepWhile(clearMotor, 0);
  }
}
