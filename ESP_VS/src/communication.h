// Communication.h

#ifndef _COMMUNICATION_h
#define _COMMUNICATION_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

// This seems to be the only way to change the hardware bluetooth port in a non hardcoded way??
#define blueTooth Serial

/** Handles communication with other units. Currently used to handle communication over bluehtooth and wire. TODO: We
 * could probably use some sort of inheritance from the serial port classes here.    */
namespace communication
{

namespace receivedValues
{
extern float steering_gain;
extern int manual_steering;
extern int motor_speed;
}

/// Parse out commands and data from a serial bus.
///@return Number of bytes received
int parseSerial(Stream& busToParse);

/** Do a loopback test. Add this to the mainloop and call it with an argument such as "Serial1". All messages on Serial1
should echo back now. Make
sure no other program unit is already reading the port. */
void loopBackTest(HardwareSerial& port);

enum class serialCommand : char
{

    AUTO = 'a',
    MANUAL = 'm',
    STOP = 's',
    TUNE = 't',
};

char* serialCommandToString(serialCommand command);

extern serialCommand current_serial_control_command;
};

#endif
