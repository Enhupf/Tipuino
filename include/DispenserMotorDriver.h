#ifndef DISPENSER_MOTOR_HH
#define DISPENSER_MOTOR_HH

#include "StepperMotorDriver.h"
#include "HomingStepperMotorMixin.h"

namespace tipuino {

  /**
   * @breif Class responsible for managing the DispenserMotor
   *
   * This is the class responsible for managing the dispenser
   * motor and tracking its state.
   */
  class DispenserMotorDriver
  : public StepperMotorDriver
  , public HomingStepperMotorMixin {

    public:

    DispenserMotorDriver(
      Hal* hal,
      const pin_t enablePinArg,
      const pin_t stepPinArg,
      const pin_t dirPinArg,
      const pin_t uartRx,
      const pin_t uartTx,
      const pin_t homePin,
      const PinValue homeDirection
    );

    void setup() override;

    /**
     * @breif Home the dispenser motor
     *
     * This method homes the dispenser motor. First it uses
     * the @ref HomingStepperMotorMixing::home() method to
     * do the initial homing. It then moves the stepper in
     * the opposite direction until the homing sensor no
     * longer triggers and some clearance is available.
     */
    void homeMotor() override;

    protected:
    HomingStepperMotorMixin::Interface& interface() override;

    private:
    Pin homePin;
    const PinValue homeDirection;
    HomingStepperMotorMixin::Interface homingInterface;
  };
}

#endif

