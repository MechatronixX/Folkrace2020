#include <Arduino.h>
#include <Wire.h>

#include "hardwareFunc.h"
#include "config.h"
#include <communication.h>
#include <Servo.h>
#include "VL53_array.h"

int incomingByte = 0; // for incoming serial data

// steeringServo(uint16_t pin, uint16_t channel, int8_t lower_angle_limit = -90, int8_t upper_angle_limit = 90,
//                 int8_t centering_offset = 0)
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

void loop()
{
    // TODO: See if we got a mode change and write something informative.
    communication::parseSerial(Serial);

    VL53array::readAll();

    switch (communication::current_serial_control_command)
    {
        case communication::serialCommand::AUTO:

            steering_angle = steering_gain * (set_distance_mm -
                                              VL53array::sensors[VL53array::FRONT_RIGHT].result.distance * meter_to_mm);

            frontSteering.setAngle(int8_t(steering_angle));

            break;

        case communication::serialCommand::MANUAL:
            frontSteering.setAngle(communication::receivedValues::manual_steering);
            // rearSteering.setAngle(communication::receivedValues::manual_steering);
            break;

        case communication::serialCommand::STOP:
            // TODO
            break;
    };
}