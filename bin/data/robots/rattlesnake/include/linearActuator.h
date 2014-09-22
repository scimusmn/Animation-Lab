#include "Arduino.h"

static bool bRunning=false;

class actuator {
private:
  int pwr,pot,dir;
  int lowLim,highLim;
  int target;
public:
  actuator(){
    target=0;
    lowLim=0;
    highLim=1024;
  }
  actuator(int pow,int Dir,int pt){
    pwr=pow;
    dir=Dir;
    pot=pt;
    target=0;
    lowLim=0;
    highLim=1024;
  }
  void setup(int pow,int Dir,int pt){
    pwr=pow;
    dir=Dir;
    pot=pt;
    pinMode(pwr,OUTPUT);
    pinMode(dir,OUTPUT);
    digitalWrite(pwr,LOW);
    digitalWrite(dir,LOW);
  }
  void setup(){
    pinMode(pwr,OUTPUT);
    pinMode(dir,OUTPUT);
    digitalWrite(pwr,LOW);
    digitalWrite(dir,LOW);
  }
  void extend(){
    digitalWrite(pwr,LOW);
    delay(200);
    digitalWrite(dir,HIGH);
    delay(200);
  }
  void retract(){
    digitalWrite(pwr,LOW);
    delay(200);
    digitalWrite(dir,LOW);
    delay(200);
  }
  void setTarget(int tar){
    target = tar;
    if(analogRead(pot)>target) retract();
    else extend();
  }
  void start(){
    digitalWrite(pwr,HIGH);
  }
  void run(){
    if(analogRead(pot)>target) retract();
    else extend();
    digitalWrite(pwr,HIGH);
    while(abs(analogRead(pot)-target)>50);
    digitalWrite(pwr,LOW);
  }
  bool & running(){
    return bRunning;
  }
  bool doneMoving(){
    bool ret=false;
    //Serial.print(abs(analogRead(pot)-target),DEC);
    //Serial.println(" is the current offset");
    if(abs(analogRead(pot)-target)<50){
      digitalWrite(pwr,LOW);
      ret=true;
    }
    else{
      digitalWrite(pwr,HIGH);
      if(analogRead(pot)>target){
        if(digitalRead(dir))
          retract();
      }
      else if(!digitalRead(dir)) extend();
    }
    return ret;
  }
  void moveTo(int pos){
    target = pos;
    run();
  }
};
