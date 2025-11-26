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
