#include "HomingStepperMotorDriver.h"

#include "platform.h"
#include "TipuinoError.h"

namespace tipuino {

  /**
   * @breif The number of times the homing routine will run before erroing out.
   */
  #define HOMING_LIMIT 100

  HomingStepperMotorDriver::HomingStepperMotorDriver(
    Hal* hal,
    const pin_t enablePin,
    const pin_t stepPin,
    const pin_t dirPin,
    const pin_t uartRx,
    const pin_t uartTx,
    const pin_t homePin,
    const PinValue homeDirection
  ) : StepperMotorDriver(hal, enablePin, stepPin, dirPin, uartRx, uartTx)
    , homePin(hal, homePin)
    , homeDirection(homeDirection)
  {
  }

  void HomingStepperMotorDriver::setup() {
    StepperMotorDriver::setup();
    delay(20);
    homePin.setup();
  }

  bool HomingStepperMotorDriver::isHome() {
    return homePin.sync() == PinValue::PinValueLow;
  }

  void HomingStepperMotorDriver::homeMotor() {
    int count = 0;
    
    while(!isHome()) {

      if (++count > HOMING_LIMIT) {
        throw TipuinoError::TipuinoError;
      }

    }
  }

}
