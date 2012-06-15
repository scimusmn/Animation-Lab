#ifndef _LED_BLINK
#define _LED_BLINK

#include "WProgram.h"

bool blinking;

class blnk {
  int dly;
  int pin;
  int speed;
public:
  blnk(){
    dly=0;
    pin=13;
  }
  void setup(){
    pinMode(pin,OUTPUT);
    blinking=false;
  }
  void start(){
    blinking=false;
  }
  void end(){
    digitalWrite(pin,0);
  }
  void call(int spd){
    speed=spd;
    blinking=true;
	if(blinking) digitalWrite(pin,(millis()/speed)%2);
  }
  void on(){
	  digitalWrite(pin,1);
  }
  void off(){
	  digitalWrite(pin,0);
  }
  void call(String spd){
	  String blinkTemp=spd;
	  if(blinkTemp.equals("FAST")) call(200);
	  else call(1000);
  }
} blink1;
    
    
#endif