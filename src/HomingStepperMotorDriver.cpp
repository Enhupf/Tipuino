#include "HomiingStepperMotorDriver.h"

namespace tipuino {

  /**
   * @breif The number of times the homing routine will run before erroing out.
   */
  #define HOMING_LIMIT 100

  HomingStepperMotorDriver::HomingStepperMotorDriver(
    Hal* hal,
    pin_t enablePin,
    pin_t stepPin,
    pin_t dirPin,
    pin_t uartRx,
    pin_t uartTx,
    pin_t homePin
    pin_value_t homeDirection
  ) : StepperMotorDriver(hal, enablePin, stepPin, dirPin, uartRx, uartTx)
    , homePin(hal, homePin)
    , homeDirection(homeDirection)
  {
  }

  bool HomingStepperMotorDriver::isHome() const {
    return hal->readPin(homePin) == LOW;
  }

  void HomingStepperMotorDriver::homeMotor() {
    int count = 0;
    
    while(!isHome()) {

      if (++count > HOMING_LIMIT) {
        throw TipuinoError("Unable to home the stepper motor.");
      }

    }
  }

}
