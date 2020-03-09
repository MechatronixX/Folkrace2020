#include <Arduino.h>
#include <Wire.h>

#include "hardwareFunc.h"
#include "config.h"
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
    // opens serial port, sets data rate to 115200 bps
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

    initServofs();
    initServobs();
    initMotor();
    Serial.println("Enter new throttle dummy");
}

void loop()
{

    // send data only when you receive data :
    if (Serial.available() > 0)
    {
        // read the incoming byte:
        int state = Serial.parseInt();
        // say what you got:
        if (state < -90)
        {
            Serial.printf("Can't execute command, too low value %d must be in range -90<->90\n", state);
            Serial.println("Enter new steer angle");
        }
        else if (state > 90)
        {
            Serial.printf("Can't execute command, too high value %d must be in range -90<->90\n", state);
            Serial.println("Enter new steer angle");
        }
        else
        {
            Serial.printf("Execute command, turn to %d\n", state);
            Serial.printf("Execute command, turn to %d\n", state);
            // steer(fs, state);
            // steer(bs, state);

            frontSteering.setAngle(state);
            rearSteering.setAngle(0);

            /*
            if (state < 0)
                motorDrive(state - 100);
            else if (state > 0)
                motorDrive(state + 100);
            else
                motorStop();
            */
            Serial.println("Enter new steer angle");
        }
    }
    else
    {
        /// Simple proportional controller for following a wall
        constexpr float set_distance_mm = 150;
        constexpr float meter_to_mm = 1000;
        constexpr float steering_gain = 0.1;
        VL53array::readAll();

        float steering_angle =
          steering_gain * (set_distance_mm - VL53array::sensors[VL53array::FRONT_RIGHT].result.distance * meter_to_mm);

        frontSteering.setAngle(int8_t(steering_angle));
    }

    // VL53array::printAll();

    //   if (Serial.available())
    //   {
    //       // read the incoming byte:
    //       int state = Serial.parseInt();
    //       // say what you got:
    //       if (state < -255)
    //       {
    //           Serial.printf("Can't execute command, too low value %d must be in "
    //                         "range -255<->255\n",
    //                         state);
    //           Serial.println("Enter new throttle value");
    //       }
    //       else if (state > 255)
    //       {
    //           Serial.printf(
    //             "Can't execute command, too high value %d must be in range "
    //             "-255<->255\n",
    //             state);
    //           Serial.println("Enter new throttle value");
    //       }
    //       else
    //       {
    //           Serial.printf("Execute command, accelerate to %d\n", state);
    //           motorDrive(state);
    //           Serial.println("Enter new throttle value");
    //       }
    //   }
    // Serial.println("------------------------------------------------");
    // delay(1000);
}