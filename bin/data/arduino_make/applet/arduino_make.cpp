<<<<<<< HEAD
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
=======
#include "WProgram.h"
#include "Servo.h"
#include "multiJack.h"



bool thru=false;

void setup(){
	thru=false;
	  (jack3).mode(INPUT);
	  (jack1).mode(SERVO);
	        
}

void loop(){

	
	
	
	
	
	

	if(!thru){
			while(1){
			if((jack3).dRead()==1){
				(jack1).sWrite(180);
				delay(4 *1000);
				(jack1).sWrite(45);
				delay(2.5 *1000);
				(jack1).sWrite(180);
			}
			if((jack3).dRead()==0){
				(jack1).sWrite(0);
			}
		}

		thru=true;
	}

}
>>>>>>> Pushing all of the changes made for EngStudio.
