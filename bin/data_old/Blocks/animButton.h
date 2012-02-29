/*
 *  animButton.h
 *  AnimationLab
 *
 *  Created by Exhibits on 2/3/12.
 *  Copyright 2012 Science Museum of Minnesota. All rights reserved.
 *
 */

#ifndef _button_
#define _button_

#include "WProgram.h"

class button {
	int pin;
  int lPin;
	bool state;
public:
	button(){
	}
	void setup(int pn,int lghtPn){
		pin=pn;
    lPin=lghtPn;
    pinMode(pin,INPUT);
    pinMode(lPin,OUTPUT);
    digitalWrite(lPin,HIGH);
	}
	void start(){
	}
	void end(){
		digitalWrite(lPin,LOW);
	}
	bool isPressed(){ return digitalRead(pin); }
} Button;

#endif