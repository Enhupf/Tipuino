#ifndef ERROR_HANDLER_HH
#define ERROR_HANDLER_HH

#include "TipuinoError.h"

#include <TimerOne.h>

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

    for(;;) {
      TipuinoError result = action();

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

  template<typename F, typename G, typename Enter = void(), typename Exit = void()>
  void retryWithTimeout(
      F&& action,
      G&& check,
      unsigned long timeout,
      TipuinoError onFailure,
      Enter&& enterError,
      Exit&& exitError
    ) {
    unsigned long start = millis();
    bool success = false;

    while(millis() - start < timeout && !(success = check())){
      action();
    }

    if(!success) {
      enterError();
      onError->handle(onFailure);
      exitError();
    }

  };

  private:
  const OnError* onError;

  };
}

#endif

