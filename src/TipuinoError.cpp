#include "TipuinoError.h"

#include <string.h>

#define TIPUINO_ERROR_SIZE 256
namespace tipuino {

  TipuinoError errorCode = TipuinoError::NoError;
  char message[TIPUINO_ERROR_SIZE] = {};

  void resetError() {
    errorCode = TipuinoError::NoError;
    message[0] = '\0';
  };

  void setError(
    const TipuinoError code,
    const char* messageArg
  ) {

    if(code == TipuinoError::NoError) {
      setError(
        TipuinoError::InvalidOperationError,
        "The 'setError' function cannot be called with 'NoError'."
      );
      return;
    }

    errorCode = code;
    strncpy(message, messageArg, TIPUINO_ERROR_SIZE);
    message[TIPUINO_ERROR_SIZE - 1] = '\0';
  }

  const TipuinoError getError() {
    return errorCode;
  }

  const char* getMessage() {
    return message;
  }
}
