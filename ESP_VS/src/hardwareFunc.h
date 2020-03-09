#ifndef hardwareFunc_h
#define hardwareFunc_h

#include <Arduino.h>
#include <ESP32Encoder.h>
#include "esp32-hal-ledc.h"
#include "config.h"

//--------------V-SENS-----------------
// #define VSense 16

extern ESP32Encoder encoder;
// extern Servo frontS;
// extern Servo backS;

// Constants to help remember the parameters
const boolean fs = 0;
const boolean bs = 1;

const int fsOffs = 10;
const int fsHLim = 120;
const int fsLLim = 40;

const int bsOffs = 30; // badly needs fysical calibration...
const int bsHLim = 120;
const int bsLLim = 40;

void initServofs(void);
void initServobs(void);
void initMotor(void);
void steer(boolean steering, int deg);
void motorDrive(int motorSpeed);
void motorStop(void);

#endif