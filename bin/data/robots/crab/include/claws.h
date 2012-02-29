/*
 *  claws.h
 *  robotBlocksVectors
 *
 *  Created by Exhibits on 5/11/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#ifndef _CRAB_CLAWS
#define _CRAB_CLAWS

#include "WProgram.h"
#include "Servo.h"

class crabClaw {
	Servo claw;
	int clawPin;
	int clawAngle;
	int open;
	int clawSpeed;
	bool running;
	int openAng,closeAng;
	bool state;
public:
	crabClaw(int pin, int opn, int cls){
		clawPin=pin;
		openAng=opn;
		closeAng=cls;
	}
	void setup(){
		running=open=0;
		claw.attach(clawPin);
	}
	void start(){
		running=open=clawSpeed=0;
	}
	void end(){
		if(open!=state){
			state=open;
			claw.write(closeAng+openAng*open);
		}
	}
	void automatic(String speed){
		running=true;
		if(speed.equals("SLOW")) clawSpeed = 900;
		else if(speed.equals("MED")) clawSpeed = 600;
		else if(speed.equals("FAST")) clawSpeed=300;
		else clawSpeed=0;
		
		open=(millis()/clawSpeed)%2;
	}
	void manual(String state){
		open=state.equals("OPEN");
		claw.write(closeAng+openAng*open);
	}
	bool isOpen(){ return open; }
} leftClaw(8,-65,130), rightClaw(4,50,10);

#endif