#ifndef ERROR_HANDLER_HH
#define ERROR_HANDLER_HH

#include "TipuinoError.h"

namespace tipuino {

  class ErrorHandler {

  public:
  class OnError {

    public:
    virtual bool handle(TipuinoError error) const = 0;
  };

  ErrorHandler(const OnError* onErrorArg) : onError(onErrorArg) {}

  template<typename F>
  void withErrorHandler(F&& action) const {

    TipuinoError result = TipuinoError::ErrorCodeNotSet;
    for(;;) {
      action(result);

      if(result == TipuinoError::NoError) {
        return;
      }
      // Todo: check if the error variable was not
      // set. If that is the case, fail fatally

      // Todo: if this returns false.
      // Tipuino must abort
      onError->handle(result);
    }
  };

  private:
  const OnError* onError;

  };
}

#endif

