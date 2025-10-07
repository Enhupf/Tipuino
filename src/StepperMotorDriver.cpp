#include "StepperMotorDriver.h"

namespace tipuino {

  #define R_SENSE 0.11f
  #define BAUD_RATE 0B00
  #define STEP_DELAY 10
  #define STEP_SPEED_DELAY 300

  StepperMotorDriver::StepperMotorDriver(
    Hal* hal,
    const pin_t enablePin,
    const pin_t stepPin,
    const pin_t dirPin,
    const pin_t uartRx,
    const pin_t uartTx
  ) : uart(uartRx, uartTx)
    , uartDriver(&uart, R_SENSE, BAUD_RATE) 
    , enablePin(hal, enablePin, HIGH)
    , stepPin(hal, stepPin, LOW)
    , dirPin(hal, dirPin)
  {
  }

  pin_value_t StepperMotorDriver::getDirection() const {
    return dirPin.value();
  }

  void StepperMotorDriver::step() {

    auto useEnable = enablePin.use();
    {
      auto useStep = stepPin.use();
      delayMicroseconds(STEP_DELAY);
    }

    // Todo, can we use UART to know when the step
    // is done?
    delayMicroseconds(STEP_SPEED_DELAY);
  }
};
