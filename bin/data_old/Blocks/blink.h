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
    if(blinking){
      digitalWrite(pin,(millis()/speed)%2);
    }
    else digitalWrite(pin,0);
  }
  void call(int spd){
    speed=spd;
    blinking=true;
  }
} blink1;
    
    
#endif
    
