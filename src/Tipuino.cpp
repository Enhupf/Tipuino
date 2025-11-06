#include "Tipuino.h"

#include "platform.h"

namespace tipuino {

  Tipuino::Tipuino(
    Hal* hal
  ) : hal(hal)
    , state(Initializing)
    , dispenserMotorDriver(
        hal,
        DISPENSER_ENABLE_PIN,
        DISPENSER_STEP_PIN,
        DISPENSER_DIR_PIN,
        DISPENSER_RX,
        DISPENSER_TX,
        DISPENSER_LIMIT_SWITCH_PIN,
        tipuino::PinValue::PinValueLow
      )
  {}

  void Tipuino::setup() {
    dispenserMotorDriver.setup();
  }

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
