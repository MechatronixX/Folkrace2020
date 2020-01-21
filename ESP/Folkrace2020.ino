/*The range readings are in units of mm.*/

#include <stdio.h>
#include <Wire.h>
#include "VL53_array.h"

constexpr uint8_t pin_left = 33;
constexpr uint8_t pin_right = 32;

void setup()
{
	Serial.begin(115200);
	Wire.begin();

	VL53array::sensors_[VL53array::ID::FRONT_LEFT] =
		VL53extra(pin_left);

	VL53array::sensors_[VL53array::ID::FRONT_RIGHT] =
		VL53extra(pin_right);

	VL53array::initAll();
}

char buffer[50];

void loop()
{
	VL53array::readAll();
	VL53array::printAll();
}