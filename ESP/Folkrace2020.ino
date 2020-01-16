/*
 Name:		Folkrace2020.ino
 Created:	1/16/2020 7:31:25 
*/

#include "pins.h"
#include <Adafruit_VL53L0X.h>

// Addresses should be 0x29 to 0x7F according to this source: https://robojax.com/learn/arduino/?vid=robojax_VL53L0X_I2C_address
// TODO: Why?? Had problems with other addresses tried, than these. Why doesnt the set function cap that in that case. 
const uint8_t adr_left =  (0x29 + 0x01);
const uint8_t adr_right = 0x2B;

Adafruit_VL53L0X vl053_left = Adafruit_VL53L0X();
Adafruit_VL53L0X vl053_right = Adafruit_VL53L0X();

void setup() 
{
	Serial.begin(115200);

	// wait until serial port opens for native USB devices
	while (!Serial) 
	{
		delay(1);
	}

	Serial.println("Starting initialization");
	pinMode(pin::VL053_left, OUTPUT);
	pinMode(pin::VL053_right, OUTPUT); 

	Serial.println("Pins are low");

	digitalWrite(pin::VL053_left, LOW);
	digitalWrite(pin::VL053_right, LOW);

	delay(500);

	//Release by tristate. 
	pinMode(pin::VL053_left, INPUT); 
	
	//digitalWrite(pin::VL053_left, HIGH); 
	delay(100);

	if (vl053_left.begin(adr_left))
	{
		Serial.println("Left started ok"); 
	}
	else
	{
		Serial.println("Left error");
	}

	digitalWrite(pin::VL053_left, LOW); 
	delay(1000);
	digitalWrite(pin::VL053_right, HIGH); 

	delay(2);

	if (vl053_right.begin(adr_right))
	{
		Serial.println("Right started ok");
	}
	else
	{
		Serial.println("Right error");
	}


	while (1);

}

uint8_t address = 0;
void loop() {
	VL53L0X_RangingMeasurementData_t measure;

	Serial.print("Reading a measurement... ");
	//lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

	if (measure.RangeStatus != 4) 
	{  // phase failures have incorrect data
		Serial.print("Distance (mm): "); 
		Serial.println(measure.RangeMilliMeter);
	}
	else 
	{
		Serial.println(" out of range ");
	}

	delay(100);
}
