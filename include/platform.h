#ifndef PLATFORM_H
#define PLATFORM_H

#include "hal/Atmega2560.h"

/* This file defines the constants that are used to
 * communicate with the hardware.
*/
#define NEOPIXEL_PIN                25
#define LCD_CLOCK                   52
#define LCD_MOSI                    51
#define LCD_RESET                   23
#define LCD_CS                      17
#define LCD_RS                      16

// --- Pin Definitions ---
// Dispenser Axis
#define DISPENSER_STEP_PIN         46
#define DISPENSER_DIR_PIN          48
#define DISPENSER_ENABLE_PIN       A8
#define DISPENSER_LIMIT_SWITCH_PIN 21
#define DISPENSER_END_SWITCH_PIN   39
#define DISPENSER_BEAM_PIN         18
#define DISPENSER_RX               65
#define DISPENSER_TX               42

// Box Axis
#define BOX_STEP_PIN               26
#define BOX_DIR_PIN                28
#define BOX_ENABLE_PIN             24
#define BOX_LIMIT_SWITCH_PIN       11
#define BOX_ENCODER_PIN            19
#define BOX_RX                     A12
#define BOX_TX                     44

// Screw Axis
#define SCREW_STEP_PIN             A6
#define SCREW_DIR_PIN              A7
#define SCREW_ENABLE_PIN           A2
#define SCREW_ENCODER_PIN          15
#define SCREW_RX                   A10
#define SCREW_TX                   A5

// Wheel Axis
#define WHEEL_STEP_PIN             A0
#define WHEEL_DIR_PIN              A1
#define WHEEL_ENABLE_PIN           38
#define WHEEL_ENCODER_PIN          2
#define WHEEL_BEAM_PIN             14
#define WHEEL_RX                   A9
#define WHEEL_TX                   40

#define HAL_CLASS tipuino::hal::Atmega2560

#endif
