// VL053_wrapper.h

#ifndef _VL053_WRAPPER_h
#define _VL053_WRAPPER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <VL53L0X.h>

struct VL53result
{
	float distance;
	uint32_t timestamp;
	bool isValid;
};

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

	VL53result result; 
	
private: 
	int8_t pin_reset_; 
};



//This value is hardcoded into the Polulu library,
//should maybe fork their library and add this to some config header. 
constexpr auto VL053invalidValue = 65535; 

//Wrapper functions that makes it a bit easier to read
//all sensors . Singleton, do not instantiate more than one 
//of these per program. Maybe should use namespace here instead.
// Template the array size as we might change the number of sensors.
namespace VL53array
{
	//We are a bit lazy and do not use enum class here, so that we easily can
	//index arrays with the thing. 
	enum ID : unsigned int
	{
		FRONT_LEFT,
		FRONT_RIGHT,
		N
	};

	//So we do not need to do the static cast all over the place
	constexpr uint8_t	num_sensors = static_cast<uint8_t>(ID::N);

	VL53extra sensors_[num_sensors];

	extern void resetAll();
	extern void printOne(uint8_t index);

	void initAll()
	{
		//Reset all sensors, then release them one by one and set their addresses. 
		//Note that we do not care about the address as long as it is 
		//unique. 
		resetAll();

		for (uint8_t i = 0; i < num_sensors; i++)
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
		for (uint8_t i = 0; i < num_sensors; i++)
		{
			auto read_result = sensors_[i].readRangeContinuousMillimeters();
			sensors_[i].result.timestamp = millis();
			sensors_[i].result.distance = 0.001f * float(read_result);
			sensors_[i].result.isValid = read_result < VL053invalidValue;
		}
	}

	void printAll()
	{
		for (uint8_t i = 0; i < num_sensors; i++)
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

	//TODO: Put anon namespace here.
	


	//Print the sensors one by one, saves a lot of memory instead of returning a string 
	//with all sensors. 
	void printOne(uint8_t index)
	{
		char buffer[200];
		sprintf(buffer, "ID: %i # D(mm) %.1f \t T(ms): t: %i ", index,
																sensors_[index].result.distance * 1000,
																sensors_[index].result.timestamp);

		//Serial.println("Hallolu");
		//Serial.print(results[index].distance * 1000);
		Serial.print(buffer);
	}

	void resetAll()
	{
		for (uint8_t i = 0; i < num_sensors; i++)
		{
			sensors_[i].shutOff();
		}
	}
	//Number of sensors times what we think we need for this. 

}//namespace VL53array
	
#endif

