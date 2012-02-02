#include "WProgram.h"
void setup();
void loop();
#define IS_ROBOT 0x06
#define ROBOT_NUMBER 0x0C
#define REQUEST_IDENT 0xff

  #include "AFMotor.h"
#include "arenaRobot.h"
  #include <AFMotor.h>
#include "arenaRobot.h"
  

  

  

  #include <AFMotor.h>
#include "arenaRobot.h"
  

  

  



bool thru=false;
bool running=false;
bool bInit=false;
unsigned long lastChange=0;

bool change=false;

void changeRun(){
  if(millis()>lastChange+250){
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
	robot.setup();
	
	
	
	robot.setup();
	
	
	

}

void loop(){
  if(change){
    
    if(running&&!thru){
      while(!robot.frontSensor(400)){
    robot.doubleStep(2,BACKWARD,SINGLE);
  }
robot.rightTurn(90 );
while(!robot.frontSensor(400)){
    robot.doubleStep(2,BACKWARD,SINGLE);
  }
robot.rightTurn(90 );
while(!robot.frontSensor(400)){
    robot.doubleStep(2,BACKWARD,SINGLE);
  }
robot.leftTurn(90 );
while(!robot.frontSensor(400)){
    robot.doubleStep(2,BACKWARD,SINGLE);
  }
robot.leftTurn(90 );
while(!robot.frontSensor(400)){
    robot.doubleStep(2,BACKWARD,SINGLE);
  }

      running=false;
      robot.end();
    }
      robot.end();
  
  
  
  

    if(!running) thru=true;
    change=false;
  }
}
