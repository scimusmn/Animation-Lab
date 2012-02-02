/*
 *  arenaRobot.h
 *  robotBlocks
 *
 *  Created by Exhibits on 7/11/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#ifndef _ARENA_ROBOT
#define _ARENA_ROBOT

#include <WProgram.h>
#include "AFMotor.h"
#include "roboSteps.h"
#include "roboSense.h"

class robut : public roboMotors, public roboSense {
  bool justChk;
public:
  robut():roboMotors(2,1),roboSense(){
    justChk=false;
  }
  void setup(){
    roboMotors::setup();
    roboSense::setup();
  }
  void start(){
    roboMotors::start();
  }
  void end(){
    roboMotors::end();
    running=false;
  } 
  
} robot;

#endif