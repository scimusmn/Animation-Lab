#include "WProgram.h"
#include "Servo.h"
#include "multiJack.h"



bool thru=false;

void setup(){
	thru=false;
	  (jack2).mode(OUTPUT);

}

void loop(){
	

	if(!thru){
			(jack2).dWrite(1);

		thru=true;
	}

}
