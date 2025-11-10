#include "StepperMotorDriver.h"

#include "platform.h"

#define R_SENSE 0.11f
#define UART_MODE 0B00
#define UART_BAUD_RATE 57600
#define STEP_DELAY 10
#define STEPPER_MICROSTEPS 16

namespace tipuino {

  StepperMotorDriver::StepperMotorDriver(
    Hal* hal,
    const pin_t enablePinArg,
    const pin_t stepPinArg,
    const pin_t dirPinArg,
    const pin_t uartRx,
    const pin_t uartTx
  ) : uart(uartRx, uartTx)
    , uartDriver(&uart, R_SENSE, UART_MODE) 
    , enablePin(hal, enablePinArg, PinValue::PinValueHigh)
    , stepPin(hal, stepPinArg, PinValue::PinValueHigh)
    , dirPin(hal, dirPinArg, PinValue::PinValueLow)
  {}

  void StepperMotorDriver::setup() {

    enablePin.setup();
    delay(20);
    stepPin.setup();
    delay(20);
    dirPin.setup();
    delay(20);

    uart.begin(UART_BAUD_RATE);
    delay(20);
    uart.listen();
    delay(20);
    uartDriver.begin();
    delay(20);
    uartDriver.toff(4);
    delay(20);
    uartDriver.rms_current(800);
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

  void StepperMotorDriver::setDirection(const PinValue dir) {
    dirPin.write(dir);
  }

  void StepperMotorDriver::step() {

    auto useEnable = enablePin.use();
    stepInternal();
  }

  void StepperMotorDriver::stepInternal() {
    auto useStep = stepPin.use();
    delayMicroseconds(STEP_DELAY);
  };
};
