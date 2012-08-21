#include "WProgram.h"
#include "Servo.h"
#include "multiJack.h"



bool thru=false;

void setup(){
	thru=false;
	  (port4).mode(OUTPUT);
	      
}

void loop(){

	if(1){
		(port4).aWrite((30 * 2.55));
		delay(1 *1000);
		(port4).aWrite((50 * 2.55));
		delay(1 *1000);
		(port4).aWrite((100 * 2.55));
		delay(1 *1000);
		(port4).aWrite((0 * 2.55));
		delay(1 *1000);

		//thru=true;
	}

}
