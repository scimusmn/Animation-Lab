#define IS_ROBOT 0x06
#define ROBOT_NUMBER 0x0C
#define REQUEST_IDENT 0xff

  #include "AFMotor.h"
#include "arenaRobot.h"


bool thru=false;
bool running=false;
bool bInit=false;
unsigned long lastChange=0;

bool change=false;

void changeRun(){
  int read=0;
  int num=20;
  for(int i=0; i<num; i++){
    if(digitalRead(2)) read++;
  }
  if(millis()>lastChange+250&&read>num*.75){
    lastChange=millis();
    robot.toggleRun();
    running=robot.isRunning();
    if(!running) robot.end();
    thru=false;
    change=true;
  }
}

void setup(){
  running=false;
  attachInterrupt(0, changeRun, RISING);
	robot.setup();

}

void loop(){
  if(change){
    
    if(running&&!thru){
      while(robot.isRunning()){ 		robot.doubleStep(2,BACKWARD,SINGLE);
}

      running=false;
      robot.end();
    }
    
    if(!running) thru=true;
    change=false;
  }
}
