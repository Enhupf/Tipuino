#ifndef STEPPER_MOTOR_DRIVER_H
#define STEPPER_MOTOR_DRIVER_H

#include <TMCStepper.h>

#include "prelude.h"

namespace tipuino {

  //! Represents a stepper motor along with supporting sensors 
  /*!
   * Class meant to represent a stepper motor. Takes care of managing
   * all the hardware resoruces belonging to the stepper motor.
  */
  class StepperMotorDriver {

    public:

    //! Main constructor of the driver.
    /*!
      \param Pin to enable/disable the stepper motor.
      \param Pin to trigger the movement of one step of the motor.
      \param Pin to coontrol the direction of the motor.
      \param Pin for the uart receive line.
      \param Pin for the uart transmission line.
    */
    StepperMotorDriver(
      pin_t enablePin,
      pin_t stepPin,
      pin_t dirPin,
      pin_t uartRx,
      pin_t uartTx
    );

    const pin_t enablePin;
    const pin_t stepPin;
    const pin_t dirPin;

    //! Moves the stepper motor a single step
    void step() const;

    void setDirection(const bool direction);

    bool direction() const;

    private:
    bool direction;
    SoftwareSerial uart;
    TMC2209Stepper uartDriver;
  };
}

#endif

