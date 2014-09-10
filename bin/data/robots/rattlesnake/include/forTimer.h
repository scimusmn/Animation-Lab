/*
 *  rbTimer.h
 *  robotBlocksVectors
 *
 *  Created by Exhibits on 5/16/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#ifndef _FOR_TIMER
#define _FOR_TIMER

#include "Arduino.h"

class forTimer {
	long timeSet;
	long delay;
	bool running;
public:
	forTimer(){
		delay=timeSet=0;
		running=false;
	}
	void start(){
	}
	void set(int dly){
		delay=dly;
		timeSet=millis();
	}
	bool isRunning(){
		bool ret=false;
		ret=millis()<timeSet+delay;
		running=ret;
		return ret;
	}
	void end(){

	}
};


#endif
