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
	}
	void end(){
		retract();
	}
	void extend(){
		open=1;
		bod.write(closeAng+openAng);
	}
	void retract(){
		open=0;
		bod.write(closeAng);
	}
	void setByIndex(int ind){
		open=ind;
		if(open) extend();
		else retract();
	}
	void manual(String state){
		open=state.equals("EXTEND");
		bod.write(closeAng+openAng*open);
	}
	bool isOpen(){ return open; }
} Body(16,85,0);

#endif