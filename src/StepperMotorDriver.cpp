#include "StepperMotorDriver.h"

namespace tipuino {

  const float rSense = 0.11f;
  const int baudRate = 0B00;

  StepperMotorDriver::StepperMotorDriver(
    pin_t enablePin,
    pin_t stepPin,
    pin_t dirPin,
    pin_t uartRx,
    pin_t uartTx
  ) : enablePin(enablePin)
    , stepPin(stepPin)
    , dirPin(dirPin)
    , uart(uartRx, uartTx)
    , uartDriver(&uart, rSense, baudRate) 
    , direction(false)
  {
    digitalWrite(dirPin, LOW);
  }

  bool direction() const {
    return direction;
  }

  void step() const {

  }
};
