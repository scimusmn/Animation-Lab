/*
 *  claws.h
 *  robotBlocksVectors
 *
 *  Created by Exhibits on 5/11/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#ifndef _FISH_MOVES
#define _FISH_MOVES

#include "WProgram.h"
#include "Servo.h"

class fishServo {
	Servo serv;
	int pin;
	int bOpen;
	int openAng,closeAng;
public:
	fishServo(int pn, int opn, int cls){
		pin=pn;
		openAng=opn;
		closeAng=cls;
	}
	void setup(){
		bOpen=0;
		serv.attach(pin);
		serv.write(closeAng);
	}
	void start(){
		bOpen=0;
	}
	void end(){
		close();
	}
	void open(){
		if(!bOpen){
			bOpen=1;
			serv.write(closeAng+openAng);
			delay(250);
		}
	}
	void close(){
		if(bOpen){
			bOpen=0;
			serv.write(closeAng);
			delay(250);
		}
	}
	void manual(int ind){
		if(ind) open();
		else close();
	}
	void manual(String state){
		if(state.equals("LEFT")) open();
		else close();
	}
	bool isOpen(){ return bOpen; }
} Flippers(3,-65,170), Tail(4,130,-65);

#endif