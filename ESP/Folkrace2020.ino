/*The range readings are in units of mm.*/

#include <stdio.h>
#include <Wire.h>
#include <VL53L0X.h> //From Polulu. TODO: Should perhaps have this one directly in the repo.


VL53L0X sensor_left;
VL53L0X sensor_right;

constexpr uint8_t pin_left = 33;
constexpr uint8_t pin_right = 32;

//Seems to be a max 7 bit address so do not exceed 0x7F
constexpr uint8_t adr_left = 1 << 0;
constexpr uint8_t adr_right = 1 << 1;

void setup()
{
	Serial.begin(9600);

	//Turn on left sensor 
	//pinMode(pin_left, INPUT); //Tri-states pin

	//Turn off sensors, inits a reset. 
	pinMode(pin_right, OUTPUT);
	digitalWrite(pin_right, LOW);

	pinMode(pin_left, OUTPUT);
	digitalWrite(pin_left, LOW);

	//Wait a little to ensure they are off. 
	delay(100);
	Wire.begin();

	//Turn on left sensor 
	pinMode(pin_left, INPUT);
	//digitalWrite(pin_left, HIGH); 
	delay(2);
	sensor_left.setAddress(adr_left);
	delay(2);

	//Turn on right sensor
	pinMode(pin_right, INPUT);
	delay(50);
	sensor_right.setAddress(adr_right);

	sensor_right.setTimeout(500);
	sensor_left.setTimeout(500);

	if (!sensor_left.init())
	{
		Serial.println("Failed to detect and initialize left sensor!");
		while (1) {}
	}

	if (!sensor_right.init())
	{
		Serial.println("Failed to detect and initialize right sensor!");
		while (1) {}
	}

	// Start continuous back-to-back mode (take readings as
	// fast as possible).  To use continuous timed mode
	// instead, provide a desired inter-measurement period in
	// ms (e.g. sensor.startContinuous(100)).
	sensor_left.startContinuous();
	sensor_right.startContinuous();
}

char buffer[50];

void loop()
{
	sprintf(buffer, "Left: %i \t \t Right: %i \n",	sensor_left.readRangeContinuousMillimeters(),
													sensor_right.readRangeContinuousMillimeters());

	Serial.print(buffer);

	if (sensor_left.timeoutOccurred() || sensor_right.timeoutOccurred())
	{
		Serial.print(" TIMEOUT");
	}
}