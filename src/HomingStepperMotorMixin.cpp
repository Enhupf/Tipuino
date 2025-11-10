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

    if(count > HOMING_LIMIT) {
      // Stepper motor was unable to find its
      // home. Report error.

      throw TipuinoError::TipuinoError;
    }

    stepperMotor->stepWhile(test, 0);
  }
}
