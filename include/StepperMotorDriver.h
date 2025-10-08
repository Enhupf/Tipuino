#ifndef STEPPER_MOTOR_DRIVER_H
#define STEPPER_MOTOR_DRIVER_H

#include <TMCStepper.h>

#include "Hal.h"

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
      Hal* hal,
      const pin_t enablePin,
      const pin_t stepPin,
      const pin_t dirPin,
      const pin_t uartRx,
      const pin_t uartTx
    );

    /**
     * @breif Moves the stepper motor a single step
     */
    void step();

    /**
     * @breif Set the movement direction of the stepper.
     *
     * This method controls the direction on which the Stepper
     * will move when the @ref tipuino::StepperMotorDriver.step method
     * is called.
     */
    void setDirection(const PinValue direction);

    PinValue getDirection() const;

    private:
    PinValue direction;
    SoftwareSerial uart;
    TMC2209Stepper uartDriver;
    Pin enablePin;
    Pin stepPin;
    Pin dirPin;
  };
}

#endif

