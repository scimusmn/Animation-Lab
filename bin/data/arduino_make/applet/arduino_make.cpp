#include "WProgram.h"
#include "Servo.h"
#include "multiJack.h"



bool thru=false;

void setup(){
	thru=false;
	  (port3).mode(SERVO);
	  (port2).mode(SERVO);

}

void loop(){
	
	

	if(!thru){
			(port3).sWrite(0);
		(port2).sWrite(45);

		thru=true;
	}

}
