#ifndef HAL_H
#define HAL_H

namespace tipuino {

  class Hal;

  class UsePin final {

    public:
    UsePin(const Hal* hal, pin_t pin, pin_value_t& value)
    : hal(hal), pin(pin), initial(value) { hal->writePin(inv(value)); value = inv(value); }

    ~UsePin() { hal->writePin(not(value)); value = inv(value); }

    private:
    const Hal* hal;
    const pin_t pin;
    pin_value_t& value;

  };

  /**
   * @breif Wrapper class for a pin
   *
   * This class aims to represent the current state of a pin.
   * Only one isntance of this class per physical pin should exist,
   * although this is not enforced in a strict sense. Furthermore,
   * the shared HAL instance can be used by any piece of the code
   * to set the Pin. Therefore, good coding practice is expectd from
   * the developers to not hijack this process.
   */
  class Pin final {
    public:
    Pin(const Hal* hal, const pin_t pin, pin_value_t initial)
    : hal(hal), pin(pin), value(value) { hal->writePin(initial); }

    /**
     * @breif Safely toggle a Pin and ensure it gets reset.
     */
    UsePin use() { return UsePin(hal, pin, value); }

    private:
    const Hal* hal;
    const pin_t pin;
    pin_value_t value;

  };

  /**
   * @breif The "Hardware Abstraction Layre" between our fimrware and the hardware.
   *
   * This class serves as an abstraction layer between the Tipuino's logic and
   * the hardware device running this logic. The main purpose of the class is to
   * avoid directly making system calls, like digitalWrite. This allows us
   * to replace this class with any implementation we like in order to write
   * uint tests.
   */
  class Hal {

    virtual Hal() = 0;
    virtual ~Hal() = 0;

    /**
     * @breif Write the given value to the given pin.
     *
     * This function abstracts the process of writing a value to a pin.
     * Subclasses are meant to implement it using the hardware specific functions
     * to perform the task.
     */
    virtual void writePin(const pin_t pin, const pin_value_t value) const = 0;
  };
}

#endif
