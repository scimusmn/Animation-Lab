#include "WProgram.h"
#include "Servo.h"
#include "multiJack.h"



bool thru=false;

void setup(){
	thru=false;
	  (jack1).mode(INPUT);
	  (jack2).mode(OUTPUT);
	  
}

void loop(){

	
	
	

	if(!thru){
			while(1){
			if((jack1).dChanged()){
				(jack2).dWrite(1);
				delay(0.5 *1000);
			}
			(jack2).dWrite(0);
		}

		thru=true;
	}

}
