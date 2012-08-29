#ifndef _LED_BLINK
#define _LED_BLINK

#include "WProgram.h"
#include "Wire.h"

class snd {
	bool playing;
	int sound;
public:
	snd(){
		playing=false;
		sound=0;
	}
	void setup(){
		Wire.begin();
	}
	void start(){
	}
	bool isPlaying(){
		Wire.requestFrom(4,1);
		while(Wire.available()){
			playing=Wire.receive();
		}
	}
	void play(int snd){
		sound=snd;
		Wire.beginTransmission(4);
		Wire.send(sound);
		Wire.endTransmission();
	}
	void call(int temp){
		if(!isPlaying()){
			play(temp);
		}
	}
	void call(String temp){
		sound=0;
		if(temp.equals("BUBBLE")) sound = 1;
		else if(temp.equals("GLUB")) sound= 2;
		if(!isPlaying()){
			play(sound);
		}
	}
	void stop(){
		if(isPlaying()){
			play(0);
		}
	}
	void end(){
		stop();
		playing=false;
	}
} Sound;
    
    
#endif
    
