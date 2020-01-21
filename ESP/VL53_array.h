// VL053_wrapper.h

#ifndef _VL053_WRAPPER_h
#define _VL053_WRAPPER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <VL53L0X.h>

///Add a pin control feature to the Polulu baseclass
// TODO: If we fork the Polulu library
//This should be added directly to that class. 
class VL53extra: public VL53L0X
{
 public:
	VL53extra(int8_t pin_reset=-1):
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


	///Tristate the sensors reset pin. 
	void triState()
	{
		if (pin_reset_ < 0) return;
		
		pinMode(pin_reset_, INPUT);
		delay(40);
	}
	
private: 
	int8_t pin_reset_; 
};

//TODO: Should this be in the VL053 class directly perhaps
struct VL053result
{
	float distance; 
	uint32_t timestamp; 
	bool isValid; 
};

//This value is hardcoded into the Polulu library,
//should maybe fork their library and add this to some config header. 
constexpr auto VL053invalidValue = 65535; 

//Wrapper functions that makes it a bit easier to read
//all sensors . Singleton, do not instantiate more than one 
//of these per program. Maybe should use namespace here instead.
// Template the array size as we might change the number of sensors.
template<size_t N>
class VL053array
{
public:
	VL053array():
	num_sensors_(0)
	{

	}
	VL053result		results[N];

	void addSensor(const VL53extra& to_add)
	{
		if (num_sensors_ > N-1)
		{
			Serial.println("VL053 buffer full");
			return; 
		}

		sensors_[num_sensors_] = to_add; 
		num_sensors_++; 
	}

	bool isEmpty()
	{
		return num_sensors_ == 0; 
	}

	void clear()
	{
		//Will make the next add operation clear out the old sensors. 
		num_sensors_ = 0; 
	}
	void initAll()
	{
		//Reset all sensors, then release them one by one and set their addresses. 
		//Note that we do not care about the address as long as it is 
		//unique. 

		resetAll();

		for (uint8_t i = 0; i < num_sensors_; i++)
		{
			sensors_[i].triState(); 
			sensors_[i].setAddress(i); 

			if (!sensors_[i].init())
			{
				Serial.println("Failed to detect and initialize sensor!");
			}
			else
			{
				Serial.println("Init ok");
			}
			sensors_[i].setTimeout(100);
			sensors_[i].startContinuous();
		}
	}


	void readAll()
	{
		for (uint8_t i = 0; i < num_sensors_; i++)
		{
			auto read_result = sensors_[i].readRangeContinuousMillimeters();
			results[i].timestamp = millis(); 
			results[i].distance = 0.001f * float(read_result); 
			results[i].isValid = read_result < VL053invalidValue;
		}
	}

	void printAll()
	{
		for (uint8_t i=0; i < num_sensors_; i++)
		{
			printOne(i); 
		}
		Serial.println();
	}

	void print(uint8_t index)
	{
		printOne(index); 
		Serial.println();
	}
private:
	uint8_t	num_sensors_;
	VL53extra sensors_[N];

	//Print the sensors one by one, saves a lot of memory instead of returning a string 
	//with all sensors. 
	void printOne(uint8_t index)
	{
		char buffer[200]; 
		sprintf(buffer, "ID: %i # D(mm) %.1f \t T(ms): t: %i ",	index,
																results[index].distance * 1000, 
																results[index].timestamp);

		//Serial.println("Hallolu");
		//Serial.print(results[index].distance * 1000);
		Serial.print(buffer);
	}

	void resetAll()
	{
		for (uint8_t i = 0; i < num_sensors_; i++)
		{
			sensors_[i].shutOff();
		}
	}
	//Number of sensors times what we think we need for this. 
};



#endif

