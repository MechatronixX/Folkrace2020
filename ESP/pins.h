// pins.h.h

#ifndef _PINS.H_h
#define _PINS.H_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

namespace pin
{
namespace VL53
{
	const uint8_t front_left = 33;
	const uint8_t front_right = 32;

}//namespace VL53
}//namespace pin


#endif

