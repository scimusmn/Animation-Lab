#include "WProgram.h"
#include "Servo.h"
#include "multiJack.h"



bool thru=false;

void setup(){
	thru=false;
	  (port1).mode(INPUT);

}

void loop(){

	if(1){
			while((port1).dRead()==0);
		  delay(10);
		  while((port1).dRead()==1);

		//thru=true;
	}

}
