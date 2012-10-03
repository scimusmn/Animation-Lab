#ifndef MULTI_JACK
#define MULTI_JACK

#include "WProgram.h"
#include <Servo.h>

enum jackModes{
  J_OUTPUT,J_INPUT,SERVO
};

class jack {
protected:
  int out,in;
  int Mode;
  int state;
  Servo jackServo;
  int dTime;
public:
  jack(int outP, int inP){
    out=outP;
    in=inP;
    pinMode(out,OUTPUT);
    pinMode(in,INPUT);
  }
  void mode(int Mod){
    Mode=Mod;
    switch(Mode){
      case OUTPUT:
        
        break;
      case INPUT:
        digitalWrite(out,LOW);
        break;
      case SERVO:
        jackServo.attach(out);
        break;
      default:
      
        break;
    }
  }
  int dRead(){
    return digitalRead(in);
  }
  int aRead(){
    return analogRead(in);
  }
  void dWrite(int val){
    digitalWrite(out,val);
  }
  void aWrite(int val){
    analogWrite(out,val);
  }
  void sWrite(int num){
    dTime=5*(abs(jackServo.read()-num));
    jackServo.write(num);
    //delay(dTime);
  }
  void sWait(){
	  delay(dTime);
	  dTime=0;
  }
  void sWriteSpeed(int pos, int speed){
<<<<<<< HEAD
    dTime=((speed==0)?35:((speed==1)?14:5));
    for(int cPos=jackServo.read(); cPos!=pos; cPos+=(pos-cPos)/abs(pos-cPos)){
      jackServo.write(cPos);
      delay(dTime);
    }
    dTime=0;
=======
	  jackServo.attach(out);
	dTime=((speed==0)?35:((speed==1)?14:5));
	for(int cPos=jackServo.read(); cPos!=pos; cPos+=(pos-cPos)/abs(pos-cPos)){
		jackServo.write(cPos);
		delay(dTime);
	}
	dTime=0;
	jackServo.detach();
>>>>>>> Re-included demos
  }
  int dChanged(){
    int ret=0;
    if(dRead()!=state) ret=1,state=dRead();
    return ret;
  }
}port4(3,14),port3(5,15),port2(6,16),port1(11,17);;

struct varble {
	int val;
	varble(){
		val=0;
	}
	int & operator()(void){
		return val;
	}
} var1,var2,var3,var4;

#endif
