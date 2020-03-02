#include <Arduino.h>
#include "hardwareFunc.h"
#include <Wire.h>
#include "VL53_array.h"
#include "pins.h"
int incomingByte = 0;    // for incoming serial data



void setup() {
  Serial.begin(115200);    // opens serial port, sets data rate to 115200 bps
  // Enable the weak pull down resistors
	// ESP32Encoder::useInternalWeakPullResistors=true;
  // encoder.clearCount();
  // encoder.attachHalfQuad(MPuls1,MPuls2);
  // frontS.attach(fsPin);
  // backS.attach(bsPin);
  // delay(1000);
  // steer(fs,0);
  // steer(bs,0);
  
  analogWriteResolution(8); // set resolution to 10 bits for all pins
  pinMode(PWMB, OUTPUT);
  pinMode(BIn1, OUTPUT);
  pinMode(BIn2, OUTPUT);

  Wire.begin();

	//Add all TOF sensors to a static array.
	VL53array::sensors[VL53array::FRONT_CENTER] =
		VL53extra(pin::VL53::front_center);
  
  VL53array::sensors[VL53array::FRONT_LEFT] =
		VL53extra(pin::VL53::front_left);

	VL53array::sensors[VL53array::FRONT_RIGHT] =
		VL53extra(pin::VL53::front_right);

  VL53array::sensors[VL53array::CENTER_LEFT] =
		VL53extra(pin::VL53::center_left);

  VL53array::sensors[VL53array::CENTER_RIGHT] =
		VL53extra(pin::VL53::center_right);

	VL53array::initAll();

  Serial.println("Enter new throttle dummy");
}

void loop() {
  // send data only when you receive data:
  // if (Serial.available() > 0) {
  
  //   // read the incoming byte:
  //   int state = Serial.parseInt();
  //   // say what you got:
  //   if (state < -90){
  //     Serial.printf("Can't execute command, too low value %d must be in range -90<->90\n", state);
  //     Serial.println("Enter new steer angle");
  //   }
  //   else if (state > 90){
  //     Serial.printf("Can't execute command, too high value %d must be in range -90<->90\n", state);
  //     Serial.println("Enter new steer angle");
  //   }
  //   else{
  //     Serial.printf("Execute command, turn to %d\n", state);
  //     Serial.printf("Execute command, turn to %d\n", state);
  //     steer(fs,state);
  //     steer(bs,state);
  //     analogWrite(PWMB,state);
  //     Serial.println("Enter new steer angle");
  //   }    
  // }
  // VL53array::readAll();
	// VL53array::printAll();
  if(Serial.available()){
    // read the incoming byte:
    int state = Serial.parseInt();
    // say what you got:
    if (state < -255){
      Serial.printf("Can't execute command, too low value %d must be in range -255<->255\n", state);
      Serial.println("Enter new throttle value");
    }
    else if (state > 255){
      Serial.printf("Can't execute command, too high value %d must be in range -255<->255\n", state);
      Serial.println("Enter new throttle value");
    }
    else{
      Serial.printf("Execute command, accelerate to %d\n", state);
      motorDrive(state);
      Serial.println("Enter new throttle value");
    }    
  }
  Serial.println("------------------------------------------------");
  delay(1000);
  
}