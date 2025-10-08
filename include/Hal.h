#ifndef HAL_H
#define HAL_H

#include "stdint.h"

namespace tipuino {

  class Hal;
  class Pin;
  enum class PinValue : uint8_t;
  enum class PinMode : uint8_t;
  using pin_t = uint8_t;

  inline PinValue inv(const PinValue value);

  class UsePin final {

    public:
    UsePin(Pin* targetPin);

    ~UsePin();

    private:
    Pin* pin;

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
    Pin(const Hal* hal, const pin_t pinNumber, const PinValue initialValue);

    Pin(const Hal* hal, const pin_t pinNumber);

    /**
     * @breif Safely toggle a Pin and ensure it gets reset.
     */
    UsePin use();

    /**
     * @breif Convenience function to invert the value of the pin.
     */
    void invValue() { write(inv(this->value)); }

    /**
     * @breif Write a specific value to the pin.
     *
     * This method simply uses the @see Hal::writePin method to update the physical voltage
     * of the pin. It also updates an internal state variable which is used to keep track
     * of the current value.
     */
    void write(const PinValue newValue);

    /**
     * @breif Get the current value of the Pin
     *
     * This method does not read the physical voltage, only returns the internal state variable.
     * If the call the @see Pin::sync method first if you expect the physical voltage to be different
     * from the internal variable. For instance, maybe the pin is attached to a beam sensor that might
     * have becomed interrupted after some action.
     */
    PinValue getValue() const { return value; }

    /**
     * @breif Sync the Pin object with the physical voltage.
     *
     * The Pin class does not observe wether the voltage of a Pin might have
     * changed due to some event in the hardware. This function will force
     * the pin object to read the current physical voltage and sync the internal
     * variable.
     */
    PinValue sync();

    private:
    const Hal* hal;
    const pin_t pin;
    PinValue value;

  };

  /**
   * @breif The "Hardware Abstraction Layer" between our fimrware and the hardware.
   *
   * This class serves as an abstraction layer between the Tipuino's logic and
   * the hardware device running this logic. The main purpose of the class is to
   * avoid directly making system calls, like digitalWrite. This allows us
   * to replace this class with any implementation we like in order to write
   * uint tests.
   */
  class Hal {

    public:
    virtual ~Hal() = 0;

    /**
     * @breif Write the given value to the given pin.
     *
     * This function abstracts the process of writing a value to a pin.
     * Subclasses are meant to implement it using the hardware specific functions
     * to perform the task.
     */
    virtual void writePin(const pin_t pin, const PinValue value) const = 0;

    /**
     * @breif Read the physical voltage of the given pin.
     *
     * This function abstracts away the process of reading a value from a pin.
     * Subclasses are meant to implement it using the platform specific functions to
     * perform this task.
     */
    virtual PinValue readPin(const pin_t pin) const = 0;

    /**
     * @breif Set the operiation mode of the given pin.
     */
    virtual void pinMode(const pin_t pin, const PinMode mode) const = 0;
  };
}

#endif
