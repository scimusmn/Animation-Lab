/*
 *  screenTest.cpp
 *  arenaBot
 *
 *  Created by Exhibits on 11/11/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "screenTest.h"
#include "../baseBlock/baseBlock.h"

extern ofColor black, white, blue, red, orange, yellow, gray;

ofTimer actionTimer;

block * currentBlock=0;

void robotTest::setup(baseBlock * t)
{
  turtle.setup("maps/map_end.jpg");
  bTesting=false;
  bRunning=false;
  bFinished=false;
  
  display.loadFont("fonts/DinC.ttf");
  display.setSize(70);
  display.setMode(OF_FONT_CENTER);
  
  endBut.setup("Edit program", 25);
  resetBut.setup("Reset virtual robot", 25);
  
  controlBar.height=endBut.h*2;
  controlBar.width=ofGetWidth();
  base=t;
}

void robotTest::parseActions(baseBlock * t)
{
  actions.clear();
  for (unsigned int i=0; i<t->blocksOn.size(); i++) {
    actions.push_back(turtleAction(&t->blocksOn[i],&turtle));
  }
}


void robotTest::startTesting(baseBlock * t)
{
  parseActions(t);
  resetTurtle();
  resetBut.setTitle("Reset virtual robot");
  bTesting=true;
}

void robotTest::stopTesting()
{
  bTesting=false;
  base->setDrawTest(true);
  actions.clear();
}

bool robotTest::isTesting()
{
  return bTesting;
}

void robotTest::startTurtle()
{
  bRunning=true;
  actionTimer.set(.01);
  actionTimer.run();
}

bool robotTest::idleTurtle()
{
  bool ret=0;
  if(actionTimer.expired()){
    for (unsigned int i=0; i<actions.size()&&!ret; i++) {
      ret=actions[i].execute();
    }
    if(!ret) pauseTurtle(),bFinished=true;;
    if(turtle.checkPoints()) pauseTurtle();
    if(turtle.completedMaze()) pauseTurtle();
  }
  return ret;
}

void robotTest::pauseTurtle()
{
  bRunning=false;
  actionTimer.pause();
  currentBlock=0;
}

void robotTest::resetTurtle()
{
  turtle.reset();
  bFinished=false;
  for (unsigned int i=0; i<actions.size(); i++) {
    actions[i].reset();
  }
}

bool robotTest::turtleIsRunning()
{
  return bRunning;
}

bool robotTest::turtleCrashed()
{
  return turtle.crashed();
}

bool robotTest::finishedActions(){
  return bFinished;
}

void robotTest::drawCurrentBlock()
{
  if(currentBlock){
    currentBlock->drawSelected();
    //currentBlock->draw();
  }
}

void robotTest::draw(int x, int y, int w, int h)
{
  ofRectangle & cBar=controlBar;
  int mapPad=25;
  
  mapArea=ofRectangle(x,y,w,h);
  
  ofSetColor(gray);
  ofRect(mapArea.x,mapArea.y,mapArea.width,mapArea.height);
  ofSetColor(black);
  drawHatching(mapArea.x,mapArea.y,mapArea.width,mapArea.height, 50,50);
  
  ofSetColor(yellow);
  ofRect(mapArea.x+mapArea.width, mapArea.y, 2, mapArea.height);
  
  drawMap(mapArea.x+mapPad,mapArea.y+mapPad,mapArea.width-mapPad*2,mapArea.height-mapPad*2);
}

void robotTest::drawMap(int x, int y, int w, int h)
{
  ofPushMatrix();
  ofTranslate(x,y, 0);
  ofScale(double(w)/(double)turtle.map.width, double(h)/double(turtle.map.height), 1);
  ofSetColor(255, 255, 255);
  turtle.map.draw(0, 0);
  
  turtle.draw(0, 0);
  ofPopMatrix();
}

void robotTest::drawForeground()
{
  if(turtle.crashed()){
    ofSetColor(black.opacity(.75));
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    
    endBut.setTextSize(35);
    resetBut.setTextSize(35);
    int maxW = max(endBut.w,resetBut.w);
    endBut.w=resetBut.w=maxW;
    
    int boxW=max(double(display.stringWidth("Robot has crashed.")),endBut.w);
    
    drawStyledBox((ofGetWidth()-boxW)/2-50,ofGetHeight()/2-200, boxW+100, ofGetHeight()/6+resetBut.h+250);
    
    ofSetColor(white);
    display.drawString("Robot has crashed.", ofGetWidth()/2, ofGetHeight()/2-100);
    
    endBut.draw((ofGetWidth()-endBut.w)/2,ofGetHeight()/2);
    ofSetColor(yellow);
    if((ofGetElapsedTimeMillis()/250)%2) 
      trimmedRect(endBut.x-5, endBut.y-5, endBut.w+10, endBut.h+10);
    endBut.draw((ofGetWidth()-endBut.w)/2,ofGetHeight()/2);
    resetBut.draw((ofGetWidth()-resetBut.w)/2, 2*ofGetHeight()/3);
  }
  
  if (turtle.completedMaze()) {
    ofSetColor(black.opacity(.75));
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    
    endBut.setTextSize(35);
    resetBut.setTextSize(35);
    base->uploadBut.setTextSize(35);
    int maxW = max(endBut.w,max(resetBut.w,base->uploadBut.w));
    endBut.w=resetBut.w=base->uploadBut.w=maxW;
    
    drawStyledBox((ofGetWidth()-base->uploadBut.w)/2-50,ofGetHeight()/3-150, base->uploadBut.w+100, ofGetHeight()/2+resetBut.h+50);
    
    ofSetColor(white);
    display.drawString("You made it!", ofGetWidth()/2, ofGetHeight()/3-50);
    
    base->uploadBut.draw((ofGetWidth()-base->uploadBut.w)/2,ofGetHeight()/3);
    ofSetColor(yellow);
    if((ofGetElapsedTimeMillis()/250)%2) 
      trimmedRect(base->uploadBut.x-5, base->uploadBut.y-5, base->uploadBut.w+10, base->uploadBut.h+10);
    base->uploadBut.draw((ofGetWidth()-base->uploadBut.w)/2,ofGetHeight()/3);
    endBut.draw((ofGetWidth()-endBut.w)/2, ofGetHeight()/2);
    resetBut.draw((ofGetWidth()-resetBut.w)/2, 2*ofGetHeight()/3);
  }
}

void robotTest::drawControlBar(int x, int y)
{
  ofRectangle & cBar=controlBar;
  cBar.width=mapArea.width;
  cBar.x=x,cBar.y=y;
  ofSetColor(black);
  ofRect(cBar);
  
  ofSetColor(gray);
  //drawHatching(cBar.x, cBar.y, cBar.width, cBar.height, 85,80);
  drawBorder(cBar);
  
  ofSetColor(yellow);
  ofRect(cBar.x+cBar.width/2, cBar.y, 2, cBar.height);
  
  endBut.setTextSize(19);
  resetBut.setTextSize(19);
  if(!turtleIsRunning()&&!turtle.completedMaze()&&!turtleCrashed()&&(ofGetElapsedTimeMillis()/250)%2) 
    trimmedRect(resetBut.x-5, resetBut.y-5, resetBut.w+10, resetBut.h+10);
  endBut.draw(cBar.x+(cBar.width/2-endBut.w)/2, cBar.y+(cBar.height-endBut.h)/2);
  resetBut.draw(cBar.x+(3*cBar.width/2-resetBut.w)/2, cBar.y+(cBar.height-resetBut.h)/2);
}

bool robotTest::mouseLockout()
{
  return isTesting();
}


bool robotTest::clickDown(int _x, int _y)
{
  if (isTesting()) {
    if(endBut.clickDown(_x, _y))
      pauseTurtle(),resetTurtle(),stopTesting(),base->setDrawTest(true);
    if(resetBut.clickDown(_x, _y)){
      if(turtleCrashed()||turtle.completedMaze()||turtleIsRunning()||finishedActions()){
        resetTurtle();
        resetBut.setTitle("Start virtual robot");
        pauseTurtle();
      }
      else {
        startTurtle();
        resetBut.setTitle("Reset virtual robot");
      }
    }
  }
  else{
    if(base->testBut.clickDown(_x,_y)){
      startTesting(base);
      startTurtle();
      base->setDrawTest(false);
    }
  }
}


bool robotTest::clickUp()
{
  endBut.clickUp();
  resetBut.clickUp();
  base->testBut.clickUp();
}

