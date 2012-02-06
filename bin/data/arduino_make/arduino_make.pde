#define IS_ROBOT 0x06
#define ROBOT_NUMBER 0x0C
#define REQUEST_IDENT 0xff



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

}

void loop(){
  if(change){
    
    if(running&&!thru){
      for(int i=0; i<6; i++){	delay(0.5 *1000);
	delay(0.5 *1000);
}

      running=false;
      robot.end();
    }
    
    if(!running) thru=true;
    change=false;
  }
}
