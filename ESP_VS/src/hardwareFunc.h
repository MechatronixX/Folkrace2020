#ifndef hardwareFunc_h
#define hardwareFunc_h

#include <Arduino.h>
#include <analogWrite.h> // For motor control
#include <ESP32Encoder.h>
#include <ESP32_Servo.h>
//#include <Servo.h>

//--------------V-SENS-----------------
// #define VSense 16

extern ESP32Encoder encoder;
extern Servo frontS;
extern Servo backS;

// Constants to help remember the parameters
const boolean fs = 0;
const boolean bs = 1;

const int fsOffs = 10;
const int fsHLim = 120;
const int fsLLim = 40;

const int bsOffs = 30; // badly needs fysical calibration...
const int bsHLim = 120;
const int bsLLim = 40;

void steer(boolean steering, int deg);
void motorDrive(int motorSpeed);
void motorStop(void);

#endif