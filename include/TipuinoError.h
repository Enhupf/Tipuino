#ifndef TIPUINO_ERROR_H
#define TIPUINO_ERROR_H

#include <stdint.h>

#define TIPUINO_ERROR_SIZE 256

namespace tipuino {

  enum class TipuinoError : uint8_t {
    NoError = 0
  , ErrorCodeNotSet = 1
  , StepperMotorError = 2
  , InvalidOperationError = 3
  , UnableToMoveWheelToClearPosition = 4
  , UnableToMoveDispenserToClearPosition = 5

  // Indicates that the home screw motor is unable
  // to move to the next position. Happens after stepping
  // for 5s and and the sensor pin not dropping to LOW
  , HomeScrewUnableToFindNextPosition = 6

  // Indicates that the home screw reached the next
  // position but it is unable to clear the gap in
  // order to begin operation. Happens after stepping
  // for 5s and the senser pin not ascending to HIGH.
  , HomeScrewUnableToClearPosition = 7
  };

  void setError(
    const TipuinoError code,
    const char* message
  );

  void resetError();

  const TipuinoError getError();

  const char* getMessage();
}
#endif
