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
			while(1){
			Flippers.manual(0);
		}

		thru=true;
	}
	Flippers.end();

}
