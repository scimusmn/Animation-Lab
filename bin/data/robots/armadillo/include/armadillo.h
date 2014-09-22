#ifndef _ARMA
#define _ARMA

#include "Arduino.h"

class cingulata {
private:
  actuator head;
  actuator tail;
  int paw;
  bool doneMoving;
public:
  cingulata(){
    tail.setup(8,9,0);
    head.setup(10,11,1);
    paw=6;
    doneMoving=false;
  }
  void digFast(){
    analogWrite(6,128);
  }
  void digSlow(){
    analogWrite(6,64);
  }
  void stopDig(){
    digitalWrite(6,LOW);
  }
    void digSpeed(int num){
        analogWrite(6,num);
    }
  void raiseHead(){
    head.moveTo(950);
  }
  void lowerHead(){
    head.moveTo(750);
  }
    void moveHead(int num){
        head.moveTo(num);
    }
    void moveTail(int num){
        tail.moveTo(num);
    }
  void raiseTail(){
    tail.moveTo(250);
  }
  void lowerTail(){
    tail.moveTo(750);
  }
} armadillo;
#endif
