#include <Arduino.h>
#include "hardwareFunc.h"
int incomingByte = 0;    // for incoming serial data



void setup() {
  Serial.begin(115200);    // opens serial port, sets data rate to 115200 bps
  //analogWriteResolution(8); // set resolution to 10 bits for all pins
  //pinMode(PWMA, OUTPUT);
  frontS.attach(fsPin);
  backS.attach(bsPin);
  delay(1000);
  steer(fs,0);
  steer(bs,0);
  Serial.println("Enter new steer angle");
}

void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
  
    // read the incoming byte:
    int state = Serial.parseInt();
    // say what you got:
    if (state < -90){
      Serial.printf("Can't execute command, too low value %d must be in range -90<->90\n", state);
      Serial.println("Enter new steer angle");
    }
    else if (state > 90){
      Serial.printf("Can't execute command, too high value %d must be in range -90<->90\n", state);
      Serial.println("Enter new steer angle");
    }
    else{
      Serial.printf("Execute command, turn to %d\n", state);
      steer(fs,state);
      steer(bs,state);
      Serial.println("Enter new steer angle");
    }    
  }
  delay(1000);
  
}