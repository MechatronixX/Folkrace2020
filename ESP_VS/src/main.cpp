#include <Arduino.h>
#include <Wire.h>

#include "hardwareFunc.h"
#include "config.h"
#include <communication.h>
#include <Servo.h>
#include "VL53_array.h"

int incomingByte = 0; // for incoming serial data

using namespace config;

// Make sure the servos are on different channels!
steeringServo frontSteering(pin::servo::fsPin, 1, servo::front_min_angle, servo::front_max_angle,
                            servo::front_centering_offs);

steeringServo rearSteering(pin::servo::bsPin, 3, servo::rear_min_angle, servo::rear_max_angle,
                           servo::rear_centering_offs);

void setup()
{
    Serial.begin(115200);
    Serial.println("INIT:");
    delay(1000);

    // Enable the weak pull down resistors10
    // ESP32Encoder::useInternalWeakPullResistors=true;
    // encoder.clearCount();
    // encoder.attachHalfQuad(MPuls1,MPuls2);

    frontSteering.Begin();
    rearSteering.Begin();

    pinMode(pin::motor::BIn1, OUTPUT);
    pinMode(pin::motor::BIn2, OUTPUT);

    Wire.begin();

    // Add all TOF sensors to a static array.
    VL53array::sensors[VL53array::FRONT_CENTER] = VL53extra(pin::VL53::front_center);
    VL53array::sensors[VL53array::FRONT_LEFT] = VL53extra(pin::VL53::front_left);
    VL53array::sensors[VL53array::FRONT_RIGHT] = VL53extra(pin::VL53::front_right);
    VL53array::sensors[VL53array::CENTER_LEFT] = VL53extra(pin::VL53::center_left);
    VL53array::sensors[VL53array::CENTER_RIGHT] = VL53extra(pin::VL53::center_right);

    VL53array::initAll();

    initMotor();
    Serial.println("Enter new throttle dummy");
}

int last_incoming_int = 0;

/// Simple proportional controller for following a wall
constexpr float set_distance_mm = 250;
constexpr float meter_to_mm = 1000;
constexpr float steering_gain = 0.3;

float steering_angle;

communication::serialCommand last_serial_control_command = communication::current_serial_control_command;

void loop()
{
    using namespace communication;
    // TODO: See if we got a mode change and write something informative.
    parseSerial(Serial);

    if (current_serial_control_command != last_serial_control_command)
    {
        Serial.print("\n Changed main control mode from ");
        Serial.print(serialCommandToString(last_serial_control_command));
        Serial.print(" to ");
        Serial.print(serialCommandToString(current_serial_control_command));
    }

    last_serial_control_command = current_serial_control_command;

    VL53array::readAll();

    switch (current_serial_control_command)
    {
        case serialCommand::AUTO:

            steering_angle = steering_gain * (set_distance_mm -
                                              VL53array::sensors[VL53array::FRONT_RIGHT].result.distance * meter_to_mm);

            frontSteering.setAngle(int8_t(steering_angle));

            break;

        case serialCommand::MANUAL:
            frontSteering.setAngle(receivedValues::manual_steering);
            motorDrive(receivedValues::motor_speed);
            // rearSteering.setAngle(communication::receivedValues::manual_steering);
            break;

        case serialCommand::STOP:
            motorStop();
            break;

        case serialCommand::TUNE:
            // Handled internally
            break;
    };
}