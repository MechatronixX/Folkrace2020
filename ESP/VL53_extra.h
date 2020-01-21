// VL53_extra.h

#ifndef _VL53_EXTRA_h
#define _VL53_EXTRA_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <VL53L0X.h> //VL53 from the Pololu library

//This value is hardcoded into the Pololu library,
//TODO: Should maybe fork their library and add this to some config header. 
constexpr auto VL053invalidValue = 65535;

//Have this separate for more compact handling of data later.
struct VL53result
{
	float distance;
	uint32_t timestamp;
	bool isValid;
};

///Add a shut off feature to the Polulu baseclass
// TODO: If we fork the Polulu library
//This should be added directly to that class. 
class VL53extra : public VL53L0X
{
public:
	///Default argument yielding an invalid control pin feature.
	//We have this defaulted approach in order to be able to init static arrays 
	//compiletime containing instances of this class. 
	VL53extra(int8_t pin_reset = -1) :
		pin_reset_(pin_reset)
	{}

	///Reset sensor by driving its reset pin low. 
	void shutOff()
	{
		if (pin_reset_ < 0) return;

		pinMode(pin_reset_, OUTPUT);
		digitalWrite(pin_reset_, LOW);
		delay(40);
	}

	void reset()
	{
		shutOff();
		triState();
	}

	///Tristate the sensors reset pin. 
	void triState()
	{
		if (pin_reset_ < 0) return;

		pinMode(pin_reset_, INPUT);
		delay(40);
	}

	VL53result result;

private:
	int8_t pin_reset_;
};


#endif

