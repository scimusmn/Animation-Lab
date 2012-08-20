#include "WProgram.h"
#include "Servo.h"
#include "multiJack.h"



bool thru=false;

void setup(){
	thru=false;
	(port3).mode(SERVO);
	(port1).mode(INPUT);

}

void loop(){

	if(!thru){
			(port3).sWrite(map((port1).aRead(),0,1024,0,180));

		thru=true;
	}

}
