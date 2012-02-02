/*
 *  sounds.h
 *  robotBlocksVectors
 *
 *  Created by Exhibits on 5/11/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#ifndef _SOUND_SYNC
#define _SOUND_SYNC

#include "WProgram.h"
#include <Wire.h>

class soundSync {
  bool bPlaying;
	int sound;
public:
	soundSync(){
		bPlaying=false;
    sound=0;
	}
	void setup(){
		Wire.begin();
	}
	void start(){
    sound=0;
		bPlaying=0;
	}
  bool isPlaying(){
    Wire.requestFrom(4,1);
    while(Wire.available()){
      bPlaying=Wire.receive();
    }
    return bPlaying; 
  }
	void end(){
    if(!isPlaying()&&sound){
      Wire.beginTransmission(4);
      Wire.send(sound);
      Wire.endTransmission();
    }
	}
	void automatic(int snd){
		sound=snd;
	}
	void play(int snd){
    sound=snd;
    //if(sound){
      Wire.beginTransmission(4);
      Wire.send(sound);
      Wire.endTransmission();
    //}
	}
  void stop(){
    Wire.beginTransmission(4);
    Wire.send(0);
    Wire.endTransmission();
  }
  void manual(String tmp,String * arr,int num)
  {
    
  }
} Sound;

#endif