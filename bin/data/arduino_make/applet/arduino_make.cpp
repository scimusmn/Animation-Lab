#include "WProgram.h"
#include "Servo.h"
#include "multiJack.h"



bool thru=false;

void setup(){
	thru=false;
	  (port4).mode(OUTPUT);
	  (port3).mode(INPUT);
	  (port1).mode(OUTPUT);
	      (port2).mode(INPUT);
	            
}

void loop(){

	if(1){
		(port4).dWrite(1);
		if((port3).dRead()==1){
			(port1).dWrite(1);
		}
		if((port3).dRead()==0){
			(port1).dWrite(0);
		}
		delay(map((port2).aRead(),0,1024,0,2000));
		(port4).dWrite(0);
		if((port3).dRead()==1){
			(port1).dWrite(1);
		}
		if((port3).dRead()==0){
			(port1).dWrite(0);
		}
		delay(map((port2).aRead(),0,1024,0,2000));

		//thru=true;
	}

}
