#include "WProgram.h"
void setup();
void loop();

#include <Servo.h>
#include "claws.h"


bool thru=false;

void setup(){
  thru=false;
	rightClaw.setup();
	
}

void loop(){
  if(!thru){
    rightClaw.start();

    for(int i=0; i<6; i++){	rightClaw.manual("CLOSE");
	delay(0.5 *1000);
	rightClaw.manual("OPEN");
	delay(0.5 *1000);
}

    rightClaw.end();

    thru=true;
  }
}
