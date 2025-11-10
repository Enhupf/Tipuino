#include "HomingStepperMotorDriver.h"

#include "platform.h"
#include "TipuinoError.h"

namespace tipuino {

  /**
   * @breif The number of times the homing routine will run before erroing out.
   */
  #define HOMING_LIMIT 100

  bool HomingStepperMotorDriver::isHome() {
    auto& self = interface();
    return self.homePin.sync() == PinValue::PinValueLow;
  }

  void HomingStepperMotorDriver::homeMotor() {
    int count = 0;
    auto& self = interface();
    
    while(!isHome()) {

      if (++count > HOMING_LIMIT) {
        throw TipuinoError::TipuinoError;
      }
      self.stepperMotor->step();
    }
  }

}
