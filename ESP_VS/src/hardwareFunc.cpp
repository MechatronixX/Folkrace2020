#include <hardwareFunc.h>
//--------------Motors-----------------

ESP32Encoder encoder;
#include <servo.h>

void initMotor(void)
{
    ledcSetup(5, 5000, 8);
    ledcAttachPin(pin::motor::PWMB, 5);
    ledcWrite(5, 0);
}

/*
  This accuates a specified servo, to a specified angle in range -90<->90:

  TODO: Should probably be deprecated
  */
void steer(boolean steering, int deg)
{
    // convert range -90-90-->0-180
    deg = deg + 90;
    if (steering == 0)
    { // front steering
        int i = deg - fsOffs;
        if (i < fsLLim)
        {
            i = fsLLim;
        }
        if (i > fsHLim)
        {
            i = fsHLim;
        }
        int value = map(i, 0, 180, 0, 8888);
        Serial.printf("Duty cycle: %d\n", value);
        ledcWrite(1, value);
    }
    else
    {
        int i = deg - bsOffs;
        if (i < bsLLim)
        {
            i = bsLLim;
        }
        if (i > bsHLim)
        {
            i = bsHLim;
        }
        int value = map(i, 0, 180, 0, 8888);
        ledcWrite(3, value);
    }
}

// /*
//   This Drives a specified motor, in a specific direction, at a specified
//   speed:
//     - motorNumber: motor1 or motor2 ---> Motor 1 or Motor 2
//     - motorSpeed: -155 to 255 ---> 0 = stop / 255 = fast
//   */
void motorDrive(int motorSpeed)
{
    boolean in1; // Relates to AIn1 or BIn1 (depending on the motor number specified)

    // Specify the Direction to turn the motor
    // Clockwise: BIn1 = HIGH and BIn2 = LOW
    // Counter-Clockwise: BIn1 = LOW and BIn2 = HIGH
    if (motorSpeed < 0)
    {
        in1 = HIGH;
        motorSpeed *= -1;
    }
    else
    {
        in1 = LOW;
    }

    // Select the motor to turn, and set the direction and the speed
    digitalWrite(pin::motor::BIn1, in1);
    digitalWrite(pin::motor::BIn2, !in1); // This is the opposite of the BIn1
    ledcWrite(5, clamp<int>(motorSpeed, 0, 255));
}

// This stops the specified motor by setting both IN pins to LOW
void motorStop(void)
{
    digitalWrite(pin::motor::BIn1, LOW);
    digitalWrite(pin::motor::BIn2, LOW);
}