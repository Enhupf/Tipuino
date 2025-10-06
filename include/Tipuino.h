#ifndef TIPUINO_H
#define TIPUINO_H

#include "HomingStepperMotorDriver.h"

namespace tipuino {

  /**
   * @breif Enum representing the states in which the Tipuino can be.
   */
  enum TipuinoState {
    Initializing /**< The initial state. The Tipuino is in an unknown state and setup must be perofrmed. */
  , Ready /**< Tipuino is ready to begin the tipping process. Awaiting for a user signal. */
  , Exit /**< Tipuino will shutdown, the firmware will halt. */
  , Error /**< Tipuino has encountered an error condition and needs assistance from the humman. One day it might beep. */
  , FatalError /**< Tipuino has encountered an error from which it cannot recover. It will alert the humman before exiting. */
  };

  class Tipuino {

    public:
    Tipuino(
      Hal* hal,
      HomingStepperMotorDriver&& dispenserMotorDriver
    );

    /**
     * @breif Main entrypoint for the Tipuino control code.
     *
     * This method will loop until the Tipuino's state is @see TipuinoState::Exit. The method will
     * essentially look at what the current state is, call the relevant method to handle that state
     * and update the state to the result of the method call.
     */
    void run();

    private:
    TipuinoState state;
    const Hal* hal;
    HomingStepperMotorDriver dispenserMotorDriver

    /**
     * @breif The main initialization method.
     *
     * The job of this method is to bring the Tipuino to a "known" intial state, regardless on what
     * state it finds itself. In particular, it is the fist method called after the Tipuino is started
     * and takes care of homing all the stepper motors (amongst other things).
     */
    TipuinoState onInitializing();
  };

}

#endif
