#include "WProgram.h"
void setup();
void loop();

#include "blink.h"


bool thru=false;

void setup(){
  thru=false;
	blink1.setup();

}

void loop(){
  if(!thru){
    
    while(1){	String blinkTemp="SLOW";
	if(blinkTemp.equals("FAST")) blink1.call(200);
	blink1.call(1000);
}

    
    thru=true;
  }
}
