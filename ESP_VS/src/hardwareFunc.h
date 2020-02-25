#ifndef hardwareFunc_h
#define hardwareFunc_h

#include <Arduino.h>
#include <analogWrite.h>      // For motor controll
#include <Servo.h>

//-----------STARTMODULE---------------
// #define startPin 13
// #define startOnOff 12

//--------------SERVO------------------
#define fsPin 19
#define bsPin 18

//--------------Motors-----------------
//Motor 1
// not used
//Motor 2
#define BIn1 4 //Direction
#define BIn2 2 //Direction 
#define PWMB 15 //Speed

//--------------V-SENS-----------------
// #define VSense 16


extern Servo frontS;
extern Servo backS;

//Constants to help remember the parameters
const boolean m1 = 0;  //for motorDrive, motorStop, motorBrake functions. Left motor
const boolean m2 = 1;  //for motorDrive, motorStop, motorBrake functions. Right motor
const boolean fs = 0;
const boolean bs = 1;

const int fsOffs = 10;
const int fsHLim = 120;
const int fsLLim = 40; 

const int bsOffs = 30;  //badly needs fysical calibration...
const int bsHLim = 120;
const int bsLLim = 40;


void steer(boolean steering,int deg);
// void motorDrive(boolean motorNumber, int motorSpeed);
// void motorStop(boolean motorNumber);

#endif