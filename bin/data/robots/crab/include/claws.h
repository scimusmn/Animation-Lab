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
	int bOpen;
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
		bOpen=0;
		claw.attach(clawPin);
		claw.write(closeAng);
	}
	void close(){
		if(bOpen>0){
			bOpen=0;
			claw.write(closeAng);
			delay(250);
		}
	}
	void open(){
		if(bOpen==0){
			bOpen=1;
			claw.write(closeAng+openAng);
			delay(250);
		}
	}
	void start(){
		bOpen=0;
	}
	void end(){
		bOpen=0;
		claw.write(closeAng);
	}
	void manual(String state){
		if(state.equals("OPEN")) open();
		else close();
	}
	void manual(int ind){
		if(ind) open();
		else close();
	}
	bool isOpen(){ return bOpen; }
} leftClaw(16,30,15), rightClaw(17,-25,113);

#endif