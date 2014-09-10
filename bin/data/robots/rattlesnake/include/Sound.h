#ifndef _SOUND
#define _SOUND

#include "Arduino.h"
#include <Wire.h>

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
                //Serial.write("sent");
		//while(sound!=0&&!isPlaying()) delay(1);
		//delay(100);
	}
	void call(int temp){
		play(temp);
		while(isPlaying());
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
    
