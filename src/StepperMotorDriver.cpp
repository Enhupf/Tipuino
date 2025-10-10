#include "StepperMotorDriver.h"

#include "platform.h"

#define R_SENSE 0.11f
#define UART_MODE 0B00
#define UART_BAUD_RATE 57600
#define STEP_DELAY 10
#define STEP_SPEED_DELAY 300
#define STEPPER_MICROSTEPS 16

namespace tipuino {

  StepperMotorDriver::StepperMotorDriver(
    Hal* hal,
    const pin_t enablePin,
    const pin_t stepPin,
    const pin_t dirPin,
    const pin_t uartRx,
    const pin_t uartTx
  ) : uart(uartRx, uartTx)
    , uartDriver(&uart, R_SENSE, UART_MODE) 
    , enablePin(hal, enablePin, PinValue::PinValueHigh)
    , stepPin(hal, stepPin, PinValue::PinValueHigh)
    , dirPin(hal, dirPin, PinValue::PinValueLow)
  {
    uart.begin(UART_BAUD_RATE);
    delay(20);
    uart.listen();
    delay(20);
    uartDriver.toff(4);
    delay(20);
    uartDriver.microsteps(STEPPER_MICROSTEPS);
    delay(20);
    uartDriver.pwm_autoscale(true);
    delay(20);
    uartDriver.pdn_disable(true);
    delay(20);
    uartDriver.I_scale_analog(false);
    delay(20);
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
