#include "WProgram.h"




bool thru=false;

void setup(){
	thru=false;
	pinMode(13,OUTPUT);
	
}

void loop(){

	

	if(!thru){
			digitalWrite(13,1);
		delay(1 *1000);
		digitalWrite(13,0);
		delay(1 *1000);

		thru=true;
	}
	
	

}
