#ifndef ATMEGA_2560_H
#define ATMEGA_2560_H

#include "Hal.h"

namespace tipuino::hal {

  class Atmega2560 : public Hal {

    public:
    Atmega2560() {}
    ~Atmega2560() {}

    void writePin(const pin_t pin, const pin_value_t value) const override;

    pin_value_t readPin(const pin_t pin) const override;
  }
}

#endif
