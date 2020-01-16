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
	const uint8_t VL053_left = 33; 
	const uint8_t VL053_right = 32; 
}


#endif

