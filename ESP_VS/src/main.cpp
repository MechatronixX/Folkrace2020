#include <Arduino.h>
#include <Wire.h>

#include "VL53_array.h"
#include "config.h"
#include "hardwareFunc.h"
#include <Servo.h>
#include <communication.h>

using namespace config;
using namespace communication;

// Make sure the servos are on different PWM channels!
steeringServo frontSteering(pin::servo::fsPin,
                            1,
                            servo::front_min_angle,
                            servo::front_max_angle,
                            servo::front_centering_offs);

steeringServo rearSteering(pin::servo::bsPin,
                           3,
                           servo::rear_min_angle,
                           servo::rear_max_angle,
                           servo::rear_centering_offs);

void
setup()
{
    Serial.begin(115200);
    Serial.println("INIT:");
    delay(1000);

    // Enable the weak pull down resistors10
    // ESP32Encoder::useInternalWeakPullResistors=true;
    // encoder.clearCount();
    // encoder.attachHalfQuad(MPuls1,MPuls2);

    frontSteering.begin();
    rearSteering.begin();

    pinMode(pin::motor::BIn1, OUTPUT);
    pinMode(pin::motor::BIn2, OUTPUT);

    Wire.begin();

    // Add all TOF sensors to a static array.
    VL53array::sensors[VL53array::FRONT_CENTER] = VL53extra(pin::VL53::front_center);
    VL53array::sensors[VL53array::FRONT_LEFT] = VL53extra(pin::VL53::front_left);
    VL53array::sensors[VL53array::FRONT_RIGHT] = VL53extra(pin::VL53::front_right);
    VL53array::sensors[VL53array::SIDE_LEFT] = VL53extra(pin::VL53::center_left);
    VL53array::sensors[VL53array::SIDE_RIGHT] = VL53extra(pin::VL53::center_right);

    VL53array::initAll();

    initMotor();

    Serial.print("\n \n Starting in mode: ");
    Serial.println(serialCommandToString(current_serial_control_command));
}

/// Simple proportional controller for following a wall
constexpr float set_distance_mm = 250;
constexpr float meter_to_mm = 1000;
constexpr float steering_gain = 0.3;

float steering_angle;

communication::serialCommand last_serial_control_command = communication::current_serial_control_command;

void
loop()
{

    // Read incoming communication and inform a user about a main mode change
    parseSerial(Serial);

    if (current_serial_control_command != last_serial_control_command)
    {
        Serial.print("\n Changed main control mode from ");
        Serial.print(serialCommandToString(last_serial_control_command));
        Serial.print(" to ");
        Serial.print(serialCommandToString(current_serial_control_command));
    }

    last_serial_control_command = current_serial_control_command;

    // Read all sensors
    VL53array::readAll();

    switch (current_serial_control_command)
    {
        case serialCommand::AUTO:

            // Simple proportional controller trying to keep a set distance to a wall
            steering_angle = steering_gain * (set_distance_mm -
                                              VL53array::sensors[VL53array::FRONT_RIGHT].result.distance * meter_to_mm);

            frontSteering.setAngle(int8_t(steering_angle));
            rearSteering.setAngle(0);

            break;

        case serialCommand::MANUAL:
            frontSteering.setAngle(receivedValues::manual_steering_front);
            rearSteering.setAngle(receivedValues::manual_steering_rear);
            motorDrive(receivedValues::motor_speed);
            break;

        case serialCommand::STOP:
            motorStop();
            frontSteering.setAngle(0);
            rearSteering.setAngle(0);

            break;

        case serialCommand::TUNE:
            // Handled internally in the communication class.
            break;
    };

    // CSV data prinouts
    {
        Serial.println(VL53array::toCSVmillimeter());
    }
}