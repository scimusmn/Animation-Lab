#ifndef _SNAKE
#define _SNAKE

#include "Arduino.h"

class serpent {
private:
  actuator headLeft;
  actuator headRight;
  int tail;
  bool doneMoving;
public:
  serpent(){
    headLeft.setup(8,9,0);
    headRight.setup(10,11,1);
    tail=6;
    doneMoving=false;
  }
  void rattle(){
    digitalWrite(6,HIGH);
  }
    void rattleSpeed(int num){
        digitalWrite(6,num);
    }
  void stopRattle(){
    digitalWrite(6,LOW);
  }
  void moveTogether(){
    doneMoving=(headLeft.doneMoving()&&headRight.doneMoving());
    while(!doneMoving){
      doneMoving=(headLeft.doneMoving()&&headRight.doneMoving());
    }
  }
  void raiseLeft(){
    headLeft.setTarget(500);
    headRight.setTarget(200);
    moveTogether();
  }
  void raiseRight(){
    headRight.setTarget(200);
    headLeft.setTarget(700);
    moveTogether();
  }
  void lowerHead(){
    headLeft.setTarget(700);
    headRight.setTarget(900);
    moveTogether();
  }
    void moveByIndex(int num){
        if(num==0) lowerHead();
        else if(num==1) raiseRight();
        else raiseLeft();
    }
} snake;
#endif
