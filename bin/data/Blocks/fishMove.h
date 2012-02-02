/*
 *  fishMove.h
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

class fishMove {
	Servo move;
	int movePin;
	int moveAngle;
	float open;
	int moveSpeed;
	bool running;
	int openAng,closeAng;
	float state;
public:
	fishMove(int pin, int opn, int cls){
		movePin=pin;
		openAng=opn;
		closeAng=cls;
	}
	void setup(){
		running=open=0;
		move.attach(movePin);
	}
	void start(){
		running=open=moveSpeed=0;
	}
	void end(){
		if(open!=state){
			state=open;
			move.write(closeAng+openAng*open);
		}
	}
	void automatic(String speed){
		running=true;
		if(speed.equals("SLOW")) moveSpeed = 900;
		else if(speed.equals("MED")) moveSpeed = 600;
		else if(speed.equals("FAST")) moveSpeed=300;
		else moveSpeed=0;
		
		open=(millis()/moveSpeed)%2;
	}
	void manual(float percent){
    open=(percent/100.);
		move.write(closeAng+openAng*open);
	}
  void manual(String pos){
    if(pos.equals("LEFT")) open=1;
    else open=0;
		move.write(closeAng+openAng*(open));
	}
	bool isOpen(){ return open; }
} flipper(3,80,90), tail(4,180,-90);

#endif