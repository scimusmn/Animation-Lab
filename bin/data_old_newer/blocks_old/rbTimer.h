/*
 *  rbTimer.h
 *  robotBlocksVectors
 *
 *  Created by Exhibits on 5/16/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#ifndef _RB_TIMER
#define _RB_TIMER

#include "WProgram.h"

class rbTimer {
	long timeSet;
	long delay;
	int numTimes;
	bool running;
	bool repeating;
	bool justEx;
	int callNum;
	int curCall;
	int isSet;
public:
	rbTimer(){
		curCall=delay=numTimes=timeSet=isSet=0;
		running=repeating=false;
	}
	void setRepeat(){
		repeating=true;
	}
	void start(){
		callNum=0;
	}
	void set(int dly){
		if((numTimes<1||repeating)&&isSet==callNum&&curCall==callNum){
			running=true;
			justEx=false;
			delay=dly;
			isSet++;
			timeSet=millis();
		}
	}
	bool isRunning(){
		bool ret=false;
		if(running&&curCall==callNum){
			ret=millis()<timeSet+delay;
			if(!ret){
				running=false;
				justEx=true;
				curCall++;
			}
		}
		callNum++;
		return ret;
	}
	void end(){
		if((running||justEx)&&curCall>=callNum){
			justEx=false;
			curCall=0;
			numTimes++;
			if(!repeating) running=false;
		}
	}
};


#endif
