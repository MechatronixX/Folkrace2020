/*The range readings are in units of mm.*/

#include "VL053_wrapper.h"
#include <stdio.h>
#include <Wire.h>
#include <VL53L0X.h> //From Polulu. TODO: Should perhaps have this one directly in the repo.
#include "VL053_wrapper.h"


constexpr uint8_t pin_left = 33;
constexpr uint8_t pin_right = 32;
VL053array<2> distance_sensors; 

void setup()
{
	Serial.begin(115200);
	Wire.begin();

	distance_sensors.addSensor(VL53extra(pin_left)); 
	distance_sensors.addSensor(VL53extra(pin_right));

	distance_sensors.initAll();

}

char buffer[50];

void loop()
{
	distance_sensors.readAll();
	distance_sensors.printAll();
}