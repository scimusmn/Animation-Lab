/*
 *  claws.h
 *  robotBlocksVectors
 *
 *  Created by Exhibits on 5/11/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#ifndef _SNAIL_BODY
#define _SNAIL_BODY

#include "WProgram.h"
#include "Servo.h"

class snailBod {
	Servo bod;
	int bodPin;
	int open;
	int openAng,closeAng;
public:
	snailBod(int pin, int opn, int cls){
		bodPin=pin;
		openAng=opn;
		closeAng=cls;
	}
	void setup(){
		open=0;
		bod.attach(bodPin);
	}
	void start(){
		open=0;
		bod.write(closeAng);
	}
	void end(){
		//retract();
		open=0;
		bod.write(closeAng);
	}
	void retract(){
		if(!open){
			open=1;
			bod.write(closeAng+openAng);
			delay(500);
		}
	}
	void extend(){
		if(open){
			open=0;
			bod.write(closeAng);
			delay(500);
		}
	}
	void setByIndex(int ind){
		if(ind) extend();
		else retract();
	}
	void manual(String state){
		if(state.equals("EXTEND")) extend();
		else retract();
	}
	bool isOpen(){ return open; }
} Body(16,80,0);

#endif