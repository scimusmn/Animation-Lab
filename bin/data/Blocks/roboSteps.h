/*
 *  roboSteps.h
 *  robotBlocks
 *
 *  Created by Exhibits on 7/11/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#ifndef _ARENA_MOT
#define _ARENA_MOT

#include <WProgram.h>
#include "AFMotor.h"

enum robotDirection {
  R_LEFT, R_RIGHT
};

class roboMotors {
protected:
  bool running;
  float robutDiameter;
  float rCirc,wCirc;
  float wheelDiameter;
  float inchesPerStep;
  int stepsPerRot;
  int speed;
  AF_Stepper leftMotor;
  AF_Stepper rightMotor;
public:
  roboMotors(int left, int right){
    stepsPerRot=200;
    leftMotor.init(stepsPerRot,left);
    rightMotor.init(stepsPerRot,right);
    robutDiameter=4;
    wheelDiameter=2.35;
    rCirc=robutDiameter*M_PI;
    wCirc=wheelDiameter*M_PI;
    inchesPerStep=wCirc/((float)stepsPerRot);
  }
  void setup(){
    running=false;
    leftMotor.setSpeed(100);
    rightMotor.setSpeed(100);
    start();
  }
  void start(){
    leftMotor.release();
    rightMotor.release();
  }
  void end(){
    leftMotor.release();
    rightMotor.release();
  }
  void doubleStep(int steps, int direction, int style) {
    while (running&&steps--) {
      rightMotor.step(1, direction, style); 
      leftMotor.step(1, direction, style); 
    }
  }
  void forward(float inches) {
    int steps=inches/inchesPerStep;
    doubleStep(steps,BACKWARD,SINGLE);
    delay(100);
  }
  void backward(float inches) {
    int steps=inches/inchesPerStep;
    doubleStep(steps,FORWARD,SINGLE);
    delay(100);
  }
  void leftStep(int steps, int style) {
    while (running&&steps--) {
      leftMotor.step(1, FORWARD, style); 
      rightMotor.step(1, BACKWARD, style); 
    }
  }
  void leftTurn(float degrees) {
    int steps=(rCirc*(degrees/360.))/inchesPerStep;
    leftStep(steps,DOUBLE);
    delay(100);
  }
  void rightStep(int steps, int style) {
    while (running&&steps--) {
      leftMotor.step(1, BACKWARD, style); 
      rightMotor.step(1, FORWARD, style); 
    }
  }
  void rightTurn(float degrees) {
    int steps=(rCirc*(degrees/360.))/inchesPerStep;
    rightStep(steps,DOUBLE);
    delay(100);
  }
  void toggleRun(){
    running=!running;
  }
  bool isRunning(){
    return running;
  }
  void startRun(){
    running=true;
  }
  void stopRun(){
    running=0;
  }
};

#endif