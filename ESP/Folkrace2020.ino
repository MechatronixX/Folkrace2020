
#include <Wire.h>
#include "VL53_array.h"
#include "pins.h"

void setup()
{
	Serial.begin(115200);
	Wire.begin();

	//Add all TOF sensors to a static array.
	VL53array::sensors[VL53array::FRONT_LEFT] =
		VL53extra(pin::VL53::front_left);

	VL53array::sensors[VL53array::FRONT_RIGHT] =
		VL53extra(pin::VL53::front_right);

	VL53array::initAll();
}

void loop()
{
	VL53array::readAll();
	VL53array::printAll();
}