#ifndef _ARENA_SENSE
#define _ARENA_SENSE

#include <WProgram.h>
//#include "Wire.h"

class roboSense {
protected:
  int frntSnsr;
  int leftSnsr;
  //int rghtSnsr;
  signed int dirScore;
  bool lastRd[5];
  bool set[7];
  byte pData;
public:
  roboSense(){
    frntSnsr=0;
    //frntRSnsr=1;
    leftSnsr=1;
    //rghtSnsr=3;
    dirScore=0;
    set[5]=set[6]=false;
    for (int i=0; i<5; i++) {
      set[i]=lastRd[i]=false;
    }
    set[3]=true;
  }
  void setup(){
  }
  void start(){
  }
  void end(){ 
  }
  bool frontSensor(int num=800){
    return analogRead(frntSnsr)>num;
  }
  bool leftSensor(int num=800){
    return analogRead(leftSnsr)>num;
  }

};

#endif