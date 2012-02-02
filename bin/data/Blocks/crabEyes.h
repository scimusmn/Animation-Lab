/*
 *  crabEyes.h
 *  robotBlocksVectors
 *
 *  Created by Exhibits on 5/11/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#ifndef _CRAB_EYES
#define _CRAB_EYES

#include "WProgram.h"

struct color {
	unsigned char r,g,b;
	color(){r=0,g=0,b=0;}
	void set(char _r, char _g, char _b){
		r=_r, g=_g, b=_b;
	}
};

int eyeState(bool r, bool g, bool b)
{
	int ret=0;
	if(r) ret+=1;
	if(g) ret+=2;
	if(b) ret+=4;
	return ret;
}

class eyes {
	color col;
	long timer;
	int rPin,gPin,bPin;
	String tempEyes;
	bool on,rSet,gSet,bSet;
public:
	eyes(){
		on=rSet=gSet=bSet=false;
		rPin=5,gPin=6,bPin=11;
	}
	void setup(){
		on=false;
	}
	void start(){
		on=false;
		//col.r=col.g=col.b=0;
	}
	void end(){
		if(on){
			analogWrite(rPin,col.r);
			analogWrite(gPin,col.g);
			analogWrite(bPin,col.b);
		}
		else {
			analogWrite(rPin,0);
			analogWrite(gPin,0);
			analogWrite(bPin,0);
		}
	}
	void changeColor(String eyeColor){
		on=true;
		if(eyeColor.equals("RED")) col.r=255;
		else if(eyeColor.equals("GREEN")) col.g=255;
		else if(eyeColor.equals("BLUE")) col.b=255;
		else on=col.r=col.g=col.b=0;
	}
	void autoColor(){
		on=true;
		if(millis()>timer){
			timer=millis()+10;
			int iS=eyeState(rSet,gSet,bSet);
			if(iS==0) col.r++;
			else if(iS==1) col.g++;
			else if(iS==3) col.r--, col.b++;
			else if(iS==6) col.g--, col.r++;
			else if(iS==5) col.g++;
			else col.r--,col.g--,col.b--;
			if(col.r<=0) rSet=false;
			else if(col.r>=254) rSet=true;
			if(col.g<=0) gSet=false;
			else if(col.g>=254) gSet=true;
			if(col.b<=0) bSet=false;
			else if(col.b>=254) bSet=true;
		}
	}
} eyes1;

#endif