#include "Tipuino.h"

namespace tipuino {

  Tipuino::Tipuino(Hal* hal) : hal(hal), state(Initializing) {};

  void Tipuino::run() {

    while(state != TipuinoState::Exit) {
      switch(state) {

        case Initializing:
          state = onInitializing();
          break;

      }
    }
  }

  TipuinoState onInitializing() {

    return TipuinoState::Ready;
  }
}
