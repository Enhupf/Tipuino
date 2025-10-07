#include "Tipuino.h"

namespace tipuino {

  Tipuino::Tipuino(
    Hal* hal,
    HomingStepperMotorDriver&& dispenserMotorDriver
  ) : hal(hal)
    , state(Initializing)
    , dispenserMotorDriver(dispenserMotorDriver)
  {}

  void Tipuino::run() {

    while(state != TipuinoState::Exit) {
      switch(state) {

        case Initializing:
          state = onInitializing();
          break;
        default:
          state = TipuinoState::FatalError;
      }
    }
  }

  TipuinoState onInitializing() {

    return TipuinoState::Ready;
  }
}
