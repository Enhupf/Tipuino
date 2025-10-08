#ifndef ATMEGA_2560_H
#define ATMEGA_2560_H

#include "Hal.h"

namespace tipuino::hal {

  class Atmega2560 : public Hal {

    public:
    Atmega2560() {}
    ~Atmega2560() override {}

    void writePin(const pin_t pin, const PinValue value) const override;

    PinValue readPin(const pin_t pin) const override;

    void pinMode(const pin_t pin, const PinMode mode) const override;
  };
}

#endif
