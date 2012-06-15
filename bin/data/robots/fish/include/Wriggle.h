/*
 *  claws.h
 *  robotBlocksVectors
 *
 *  Created by Exhibits on 5/11/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#ifndef _FISH_WRIGG
#define _FISH_WRIGG

#include "WProgram.h"
#include "fetController.h"

class fishSkin {
	fetController skin;
	int pin;
public:
	fishSkin(int pn){
		pin=pn;
	}
	void setup(){
		skin.setup(pin);
	}
	void start(){
		skin.start();
	}
	void end(){
		skin.end();
	}
	void manual(int ind){
		if(ind) skin.on();
		else skin.off();
	}
	void manual(String state){
		skin.setSpeed(state);
	}
} Wriggle(11);

#endif