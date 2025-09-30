#ifndef TIPUINO_H
#define TIPUINO_H

namespace tipuino {

  /**
   * @breif Enum representing the states in which the Tipuino can be.
   */
  enum TipuinoState {
    Initializing /**< The initial state. The Tipuino is in an unknown state and setup must be perofrmed. */
  , Ready /**< Tipuino is ready to begin the tipping process. Awaiting for a user signal. */
  , Exit /**< Tipuino will shutdown, the firmware will halt. */
  };

  class Tipuino {

    public:
    Tipuino(Hal* hal);

    void run();

    private:
    TipuinoState state;
  };

}

#endif
