#include "StepperMotorDriver.h"

namespace tipuino {

  #define R_SENSE 0.11f
  #define BAUD_RATE 0B00
  #define STEP_DELAY 10
  #define STEP_SPEED_DELAY 300

  StepperMotorDriver::StepperMotorDriver(
    Hal* hal,
    pin_t enablePin,
    pin_t stepPin,
    pin_t dirPin,
    pin_t uartRx,
    pin_t uartTx
  ) : enablePin(hal, enablePin, HIGH)
    , stepPin(hal, stepPin, LOW)
    , dirPin(hal, dirPin)
    , uart(uartRx, uartTx)
    , uartDriver(&uart, R_SENSE, BAUD_RATE) 
  {
  }

  pin_value_t direction() const {
    return dirPin.value();
  }

  void step() const {

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
