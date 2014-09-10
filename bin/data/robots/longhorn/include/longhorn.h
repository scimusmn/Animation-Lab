#ifndef _LONGHORN
#define _LONGHORN

#include "Arduino.h"

class bovine {
private:
  actuator vertHead;
  actuator horizHead;
  bool doneMoving;
public:
  bovine(){
    horizHead.setup(8,9,0);
    vertHead.setup(10,11,1);
    doneMoving=false;
  }
  void raiseHead(){
    vertHead.moveTo(700);
  }
  void lowerHead(){
    vertHead.moveTo(950);
  }
    void moveHoriz(int num){
        horizHead.moveTo(num);
    }
    void moveVert(int num){
        vertHead.moveTo(num);
    }
  void headLeft(){
    horizHead.moveTo(950);
  }
  void headRight(){
    horizHead.moveTo(700);
  }
} longhorn;
#endif
