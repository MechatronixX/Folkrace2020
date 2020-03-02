#ifndef hardwareFunc_h
#define hardwareFunc_h

#include <Arduino.h>
#include <analogWrite.h>      // For motor controll
#include <ESP32Encoder.h>
#include <Servo.h>

//-----------STARTMODULE---------------
// #define startPin 13
// #define startOnOff 12

//--------------SERVO------------------
#define fsPin 19
#define bsPin 18

//--------------Motors-----------------

//Motor 2
#define BIn1 4 //Direction
#define BIn2 2 //Direction 
#define PWMB 15 //Speed
#define MPuls1 5
#define MPuls2 23
//--------------V-SENS-----------------
// #define VSense 16

extern ESP32Encoder encoder;
extern Servo frontS;
extern Servo backS;

//Constants to help remember the parameters
const boolean fs = 0;
const boolean bs = 1;

const int fsOffs = 10;
const int fsHLim = 120;
const int fsLLim = 40; 

const int bsOffs = 30;  //badly needs fysical calibration...
const int bsHLim = 120;
const int bsLLim = 40;


void steer(boolean steering,int deg);
void motorDrive(int motorSpeed);
void motorStop(void);

#endif