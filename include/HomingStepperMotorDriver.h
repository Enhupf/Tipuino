#ifndef HOMING_STEPPER_MOTOR_DRIVER_H
#define HOMING_STEPPER_MOTOR_DRIVER_H

namespace tipuino {

  //! Represents a stepper motor whith a "home" position
  /*!
   * The Tipuino has several stepper motors that can be "homed".
   * In the physical world, there is a physical swithc that will
   * produce a signal if the stepper motor reaches the home position.
  */
  class HomingStepperMotorDriver : public StepperMotorDriver {

    public:
    //! Main constructor of the homing motor driver.
    /*!
      \param
      \param
      \param
      \param
      \param
      \param The pin that produces the signal indicating that the motor is "homed".
    */
    HomingStepperMotorDriver(
      pin_t enablePin,
      pin_t stepPin,
      pin_t dirPin,
      pin_t uartRx,
      pin_t uartTx,
      pin_t homePin
    );
  };

  bool isHome() const;

  const pin_t homePin;
}

#endif
