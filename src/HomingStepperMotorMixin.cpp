#include "HomingStepperMotorMixin.h"

#include "platform.h"
#include "TipuinoError.h"

namespace tipuino {

  /**
   * @breif The number of times the homing routine will run before erroing out.
   */
  #define HOMING_LIMIT 100

  bool HomingStepperMotorMixin::isHome() {
    return interface().homePin.sync() == PinValue::PinValueLow;
  }

  void HomingStepperMotorMixin::homeMotor() {

    auto& mixin = interface();
    auto* stepperMotor = mixin.stepperMotor;
    stepperMotor->setDirection(mixin.homeDirection);
    int count = 0;
    auto test = [this, &count] {
      count++;
      return !(count > HOMING_LIMIT || isHome());
    };

    stepperMotor->stepWhile(test, 0);

    if(count > HOMING_LIMIT) {
      setError(
        TipuinoError::StepperMotorError,
        "Unable to home stepper motor."
      );
    }
  }
}
