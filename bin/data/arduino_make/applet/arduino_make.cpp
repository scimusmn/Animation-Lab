#include "WProgram.h"
#include "Servo.h"
#include "fishMoves.h"


bool thru=false;

void setup(){
	thru=false;
	Flippers.setup();
	
}

void loop(){
Flippers.start();
	
	if(!thru){
		Flippers.manual(0);
		delay(1 *1000);
		Flippers.manual(1);
		delay(1 *1000);

		thru=true;
	}
	Flippers.end();
	
}
