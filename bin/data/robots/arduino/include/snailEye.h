/*
 *  claws.h
 *  robotBlocksVectors
 *
 *  Created by Exhibits on 5/11/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#ifndef _SNAIL_EYE
#define _SNAIL_EYE

#include "WProgram.h"
#include "fetController.h"

class snailEye {
	fetController eye;
	int pin;
public:
	snailEye(int pn){
		pin=pn;
	}
	void setup(){
		eye.setup(pin);
	}
	void start(){
		eye.start();
	}
	void end(){
		eye.end();
	}
	void manual(String state){
		if(state.equals("BACK")) eye.on();
		else eye.off();
	}
} leftEye(2),rightEye(3);

#endif