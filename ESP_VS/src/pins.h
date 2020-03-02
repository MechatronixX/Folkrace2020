// pins.h.h

#ifndef _PINS_H_h
#define _PINS_H_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

namespace pin
{
namespace VL53
{
const uint8_t front_center = 32;
const uint8_t front_left = 33;
const uint8_t front_right = 25;
const uint8_t center_left = 26;
const uint8_t center_right = 27;

} // namespace VL53

//-----------STARTMODULE---------------
// #define startPin 13
// #define startOnOff 12

//--------------SERVO------------------

namespace servo
{
constexpr uint8_t fsPin = 19;
constexpr uint8_t bsPin = 18;
}

namespace motor
{
constexpr uint8_t BIn1 = 4;
constexpr uint8_t BIn2 = 2;
constexpr uint8_t PWMB = 15; // Speed
constexpr uint8_t MPuls1 = 5;
constexpr uint8_t MPuls2 = 23;
}

} // namespace pin

#endif
