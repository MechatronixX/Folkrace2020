//
//
//

#include "Communication.h"

namespace communication
{

serialCommand current_serial_control_command = serialCommand::MANUAL;

// Variables received
namespace receivedValues
{
float steering_gain;
int manual_steering;
int motor_speed;
}

char* serialCommandToString(serialCommand command)
{
    switch (command)
    {
        case serialCommand::STOP:
            return "STOP";
            break;
        case serialCommand::AUTO:
            return "AUTO";
            break;
        case serialCommand::MANUAL:
            return "MANUAL";
            break;
    }
}

float parseOutFloat(Stream& busToParse, bool use_delay = true)
{
    // TODO: In some arduino implementations you have to do Serial.print here instead.
    busToParse.println("Enter a value. ");

    while (busToParse.available() < 1)
    {
    };

    if (use_delay)
        delay(2000);

    constexpr uint8_t buffer_size = 12;
    char buffer[buffer_size];
    uint8_t buffer_index = 0;
    while (busToParse.available() && buffer_index < buffer_size - 1)
    {
        buffer[buffer_index++] = busToParse.read();
    }

    float return_val = atof(buffer);

    busToParse.println("I got value: ");
    busToParse.println(return_val);

    return return_val;
}

int parseSerial(Stream& busToParse)
{
    int bytesRead = 0;

    while (busToParse.available())
    {
        bytesRead++;

        switch (static_cast<serialCommand>(busToParse.read()))
        {
            case serialCommand::AUTO:
                current_serial_control_command = serialCommand::AUTO;
                break;
            case serialCommand::STOP:
                current_serial_control_command = serialCommand::STOP;
                break;
            case serialCommand::MANUAL:
                current_serial_control_command = serialCommand::MANUAL;
                break;

            case serialCommand::TUNE:

                blueTooth.println("In parameter tuning mode. Enter 'g' for steering gain, 'm' for motor speed and 's' "
                                  "for steering angle ");

                // Wait for a full float to arrive
                while (busToParse.available() < 1)
                {
                };

                switch (busToParse.read())
                {
                    case 'g':
                        receivedValues::steering_gain = parseOutFloat(busToParse);
                        break;
                    case 'm':
                        blueTooth.println("Motor.");
                        receivedValues::motor_speed = static_cast<int>(parseOutFloat(busToParse));
                        break;
                    case 's':
                        blueTooth.println("Steering.");
                        receivedValues::manual_steering = static_cast<int>(parseOutFloat(busToParse));
                        break;
                    default:
                        break;
                };

                // Assume this is a non time critical application and use the delay to hold the debug messages for a
                // while.
                delay(1000);

                break;
        };
    }

    return bytesRead;
}

void loopBackTest(HardwareSerial& port)
{
    if (!port.available())
    {
        return;
    }
    else
    {
        port.print("Reveived: ");
        port.println(port.readString());
    }
}
} // namespace communication