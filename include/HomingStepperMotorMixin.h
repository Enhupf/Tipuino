#ifndef HOMING_STEPPER_MOTOR_DRIVER_H
#define HOMING_STEPPER_MOTOR_DRIVER_H

#include "Hal.h"
#include "StepperMotorDriver.h"

namespace tipuino {

  /**
   * @breif Represents a stepper motor whith a "home" position
   *
   * The Tipuino has several stepper motors that can be "homed".
   * In the physical world, there is a physical swithc that will
   * produce a signal if the stepper motor reaches the home position.
  */
  class HomingStepperMotorMixin {

    public:
    /**
     * @breif Values that must be supplied to use the mixin.
     *
     * @param stepperMotor The motor which can be homed.
     * @param homePin The pin which produces the signal indicating the motor is homed.
     * @param homeDirection The direction the motor must move to reach the "home" poisiton
     */
    struct Interface {
        StepperMotorDriver* stepperMotor;
        Pin& homePin;
        const PinValue& homeDirection;
    } Interface;

    HomingStepperMotorMixin() {}

    /**
     * @breif Check if the stepper motor is "homed"
     *
     * This function reads the physical voltage of the homing switch. It then compares if the
     * voltage is set to @see LOW
     */
    bool isHome();

    /**
     * @breif The main homing routine.
     *
     * This method wil trigger the homing routine. This will move the stepper
     * unitl it reaches the "home" state as established by it's accompanying
     * sensor.
     */
    void homeMotor();

    protected:
    virtual Interface& interface() = 0;
  };
}

#endif
