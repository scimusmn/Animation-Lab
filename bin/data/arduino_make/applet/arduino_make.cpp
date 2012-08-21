#include "WProgram.h"
#include "Servo.h"
#include "multiJack.h"



bool thru=false;

void setup(){
	thru=false;
	  (port3).mode(SERVO);

}

void loop(){

	if(1){
			(port3).sWrite(135);

		//thru=true;
	}

}
