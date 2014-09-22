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
			playing=Wire.read();
		}
		return playing;
	}
	void play(int snd){
		sound=snd;
		Wire.beginTransmission(4);
		Wire.write(sound);
		Wire.endTransmission();
		//while(sound!=0&&!isPlaying()) delay(1);
		//delay(100);
	}
	void call(int temp){
		if(!isPlaying()){
			play(temp);
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
    
