// VL053_wrapper.h

#ifndef _VL053_WRAPPER_h
#define _VL053_WRAPPER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "VL53_extra.h"

/// Wrapper functions that makes it a bit easier to read
/// all sensors in one single array. Since we only
/// instaniate one such array per program,
/// and all sensors are known compiletime, it seems
/// to make sense to use a namespace for this instead
/// of a class/template class.
namespace VL53array
{
// Timeout in ms for all sensors.
constexpr uint16_t default_timeout = 40;

// Define all VL53 sensors used in the project in this enumeration.
// We are a bit lazy and do not use enum class here, so that we easily can
// index arrays with the thing.
enum ID : unsigned int
{
    FRONT_CENTER,
    FRONT_LEFT,
    FRONT_RIGHT,
    CENTER_LEFT,
    CENTER_RIGHT,
    N_SENSORS // Good thing with the enum approach is knowing compile time all
              // array sizes.
};

// Keep this in case we refactor to an enum class again.
constexpr uint8_t num_sensors = static_cast<uint8_t>(ID::N_SENSORS);

VL53extra sensors[num_sensors];

extern void shutAllOff();
extern void printOne(uint8_t index);

/// Reset all sensor addresses and assign them a unique address.
void initAll()
{
    // Reset all sensors, then release them one by one and set their addresses.
    // Note that we do not care about the address as long as it is
    // unique.
    shutAllOff();

    for (uint8_t i = 0; i < num_sensors; i++)
    {
        sensors[i].triState();
        sensors[i].setAddress(i);

        if (!sensors[i].init())
        {
            Serial.println("Failed to detect and initialize VL053LX!");
        }
        else
        {
            Serial.println("Init of VL53L0X ok.");
        }
        sensors[i].setTimeout(default_timeout);
        sensors[i].startContinuous();
    }
}

void readAll()
{
    for (uint8_t i = 0; i < num_sensors; i++)
    {
        auto read_result = sensors[i].readRangeContinuousMillimeters();
        sensors[i].result.timestamp = millis();
        sensors[i].result.distance = 0.001f * float(read_result);
        // If a sensor has a timeout, a default error value is returned.
        sensors[i].result.isValid = read_result < VL053invalidValue;
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

// TODO: Put anon namespace here.

// Print the sensors one by one, saves a lot of memory instead of returning a
// string
// with all sensors.
void printOne(uint8_t index)
{
    char buffer[200];
    sprintf(buffer, "ID: %i # D(mm) %.1f \t T(ms): t: %i ", index, sensors[index].result.distance * 1000,
            sensors[index].result.timestamp);

    // Serial.println("Hallolu");
    // Serial.print(results[index].distance * 1000);
    Serial.print(buffer);
}

void shutAllOff()
{
    for (uint8_t i = 0; i < num_sensors; i++)
    {
        sensors[i].shutOff();
    }
}
// Number of sensors times what we think we need for this.

} // namespace VL53array

#endif