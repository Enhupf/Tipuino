#include "StepperMotorDriver.h"

#include "platform.h"

#define R_SENSE 0.11f
#define BAUD_RATE 0B00
#define STEP_DELAY 10
#define STEP_SPEED_DELAY 300

namespace tipuino {

  StepperMotorDriver::StepperMotorDriver(
    Hal* hal,
    const pin_t enablePin,
    const pin_t stepPin,
    const pin_t dirPin,
    const pin_t uartRx,
    const pin_t uartTx
  ) : uart(uartRx, uartTx)
    , uartDriver(&uart, R_SENSE, BAUD_RATE) 
    , enablePin(hal, enablePin, PinValue::PinValueHigh)
    , stepPin(hal, stepPin, PinValue::PinValueHigh)
    , dirPin(hal, dirPin, PinValue::PinValueLow)
  {
  }

  PinValue StepperMotorDriver::getDirection() const {
    return dirPin.getValue();
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
