#include "WProgram.h"
#include "Servo.h"
#include "multiJack.h"



bool thru=false;

void setup(){
	thru=false;
	  (jack1).mode(INPUT);
	  (jack3).mode(SERVO);
	  (jack2).mode(OUTPUT);
	    
}

void loop(){

	
	
	
	
	

	if(!thru){
			while(1){
			while((jack1).dRead()==0);
			  delay(10);
			  while((jack1).dRead()==1);
			(jack3).sWrite(0);
			delay(0.5 *1000);
			(jack2).dWrite(1);
			delay(0.5 *1000);
			(jack2).dWrite(0);
			(jack3).sWrite(180);
		}

		thru=true;
	}

}
