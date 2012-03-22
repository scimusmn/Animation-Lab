/*
 *  fishMove.h
 *  robotBlocksVectors
 *
 *  Created by Exhibits on 5/11/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#ifndef _FET_CONTROL
#define _FET_CONTROL

#include "WProgram.h"

class fetController {
	int fetPin;
  bool bAnalogPin;
  bool bRunning;
  int switchSpeed;
public:
	fetController(){
    bRunning=bAnalogPin=false;
  }
	void setup(int pin){
    if(pin==3||pin==5||pin==6||(pin>=9&&pin<=11)) bAnalogPin=true;
		fetPin=pin;
    pinMode(fetPin,OUTPUT);
	}
	void start(){
		bRunning=false;
	}
	void end(){
		if(bRunning) off();
	}
	void setSpeed(String speed){
		bRunning=true;
		int switchSpeed=0;
		if(speed.equals("SLOW")) switchSpeed = 100;
		else if(speed.equals("MED")) switchSpeed = 175;
		else if(speed.equals("FAST")) switchSpeed=255;
		else bRunning=false;
    
    if(bAnalogPin) analogWrite(fetPin,switchSpeed);
    else digitalWrite(fetPin,((switchSpeed)?1:0));
	}

  void on(){
    bRunning=true;
    digitalWrite(fetPin,HIGH);
  }
  void off(){
    bRunning=false;
    if(bAnalogPin) setSpeed("OFF");
    else digitalWrite(fetPin,LOW);
  }
};

#endif