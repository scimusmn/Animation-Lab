/*
 *  claws.h
 *  robotBlocksVectors
 *
 *  Created by Exhibits on 5/11/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#ifndef _SNAIL_TAIL
#define _SNAIL_TAIL

#include "WProgram.h"
#include "fetController.h"

class snailTail {
	fetController tail;
	int pin;
public:
	snailTail(int pn){
		pin=pn;
	}
	void setup(){
		tail.setup(pin);
	}
	void start(){
		tail.start();
	}
	void end(){
		tail.end();
	}
	void manual(String state){
		if(state.equals("ON")) tail.on();
		else tail.off();
	}
} Tail(17);

#endif