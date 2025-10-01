#ifndef HOMING_STEPPER_MOTOR_DRIVER_H
#define HOMING_STEPPER_MOTOR_DRIVER_H

#include "Hal.h"

namespace tipuino {

  /**
   * @breif Represents a stepper motor whith a "home" position
   *
   * The Tipuino has several stepper motors that can be "homed".
   * In the physical world, there is a physical swithc that will
   * produce a signal if the stepper motor reaches the home position.
  */
  class HomingStepperMotorDriver : public StepperMotorDriver {

    public:
    /** @breif Main constructor of the homing motor driver.
     *
     * @param
     * @param
     * @param
     * @param
     * @param
     * @param
     * @param The pin that produces the signal indicating that the motor is "homed".
     * @param The direction that the stepper motor needs to move in order to "home" itself.
    */
    HomingStepperMotorDriver(
      Hal* hal,
      pin_t enablePin,
      pin_t stepPin,
      pin_t dirPin,
      pin_t uartRx,
      pin_t uartTx,
      pin_t homePin,
      pin_value_t homeDirection
    );
  };

  /**
   * @breif Check if the stepper motor is "homed"
   *
   * This function reads the physical voltage of the homing switch. It then compares if the
   * voltage is set to @see LOW
   */
  bool isHome() const;

  void homeMotor();

  private:
  Pin homePin;
  cosnt pin_value_t homeDirection;
}

#endif
