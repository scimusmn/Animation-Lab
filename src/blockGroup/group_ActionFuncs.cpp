/*
 *  groupActionFuncs.cpp
 *  arenaBot
 *
 *  Created by Exhibits on 11/1/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */


#include "blockGroup.h"


/*extern int pixPerInch;

block * bGroup::nextActionBlock(block & b, bool skip)
{
  block * ret=0;
  for (unsigned int i=0; i<b.blocksIn.size()&&!ret; i++) {
    if(!b.blocksIn[i].action.bExecuted){
      ret=&b.blocksIn[i];
    }
  }
  for (unsigned int i=0; i<b.blocksOn.size()&&!ret; i++) {
    if(!b.blocksOn[i].action.bExecuted)
      ret=&b.blocksOn[i];
  }
  return ret;
}

void bGroup::parseActions()
{
  for (unsigned int i=0; i<base.blocksOn.size(); i++) {
    base.blocksOn[i].parseAction();
  }
}

void resetActions(block & t, bool skip=false)
{
  if(!skip){
    t.action.bParsed=t.action.bExecuted=false;
    t.action.bData=t.action.data[1]=0;
  }
  for (unsigned int i=0; i<t.blocksIn.size(); i++) {
    resetActions(t.blocksIn[i]);
  }
  for (unsigned int i=0; i<t.blocksOn.size(); i++) {
    resetActions(t.blocksOn[i]);
  }
}

void bGroup::startSequence()
{
  parseActions();
  resetActions(base);
  turtle.pos.x=turtle.start.x;
  turtle.pos.y=turtle.start.y;
  turtle.bearing.x=0;
  turtle.bearing.y=-1;
  turtle.clear();
  bSequencePlay=true;
}

bool bGroup::checkAgainstImage(){
  
}

bool bGroup::interpretDataStr(string str)
{
  bool ret=false;
  vector<string> spl=ofSplitString(str, "$[]()");
  bool negate=false;
  for (unsigned int i=0; i<spl.size(); i++) {
    cout << spl[i] << endl;
    if(spl[i]=="!") negate=true;
    else if(spl[i]=="front") ret=turtle.frontIsClear(ofToFloat(spl[i+1])*pixPerInch, mapp);
    else if(spl[i]=="frontLeft") ret=turtle.frontLeftIsClear(ofToFloat(spl[i+1])*pixPerInch, mapp);
    else if(spl[i]=="frontRight") ret=turtle.frontRightIsClear(ofToFloat(spl[i+1])*pixPerInch, mapp);
    else if(spl[i]=="leftPath") ret=turtle.leftIsClear(ofToFloat(spl[i+1])*pixPerInch, mapp);
    else if(spl[i]=="rightPath") ret=turtle.rightIsClear(ofToFloat(spl[i+1])*pixPerInch, mapp);
    else if(spl[i]=="forever") ret=1;
  }
  if(negate) ret=!ret;
  return ret;
}

bool bGroup::idleSequence(block * search)
{
  bool ret=false;
  if(bSequencePlay){
    if(search==&base) ret=true;
    if(actionTime.expired()){
      block * t;
      if(!currentTest||currentTest->action.bExecuted||currentTest==search)
        currentTest=t=nextActionBlock(*search);
      else t=currentTest;
      if(t){
        block & nxt=*t;
        blockAction & bA=nxt.action;
        ret=true;
        switch (bA.act) {
          case OF_BLOCK_MOVE:
            if(bA.data[0]<0){
              if(bA.data[0]<bA.data[1]){
                turtle.move(-1);
                bA.data[1]-=1;
              }
              else bA.bExecuted=true;
            }
            else {
              if(bA.data[0]>bA.data[1]){
                turtle.move(1);
                bA.data[1]+=1;
              }
              else bA.bExecuted=true;
            }
            actionTime.set(0.01);
            break;
            
            
          case OF_BLOCK_TURN:
            if(bA.data[0]<0){
              if(bA.data[0]<bA.data[1]){
                turtle.turn(-1);
                bA.data[1]-=1;
              }
              else bA.bExecuted=true;
            }
            else{
              if(bA.data[0]>bA.data[1]){
                turtle.turn(1);
                bA.data[1]+=1;
              }
              else bA.bExecuted=true;
            }
            actionTime.set(0.01);
            break;
            
            
          case OF_BLOCK_WHILE:
            if(interpretDataStr(bA.dataStr)){
              if(!idleSequence(&nxt)){
                resetActions(nxt,true);
              }
            }
            else bA.bExecuted=true;
            break;
            
            
          case OF_BLOCK_IF:
            if(interpretDataStr(bA.dataStr)||bA.bData){
              bA.bData=true;
              if(!idleSequence(&nxt))
                bA.bExecuted=true,bA.bData;
            }
            else bA.bExecuted=true;
            break;
            
            
          case OF_BLOCK_REPEAT:
            if(!bA.dataStr.compare("$forever")){
              if(interpretDataStr(bA.dataStr)){
                if(!idleSequence(&nxt))
                  resetActions(nxt,true);
                ret=true;
              }
              else resetActions(nxt);
            }
            else{
              if(bA.data[0]>bA.data[1]){
                if(!idleSequence(&nxt)){
                  bA.data[1]++;
                  resetActions(nxt,true);
                }
              }
              else bA.bExecuted=true,resetActions(nxt,true);
            }
            break;
            
            
          default:
            ret=false;
            break;
        }
      }
      else {
        ret=false;
        //resetActions(base);
      }
    }
  }
  return ret;
}

void bGroup::drawCurrentBlock()
{
  if(currentTest){
    ofSetColor(255, 255, 255);
    currentTest->drawSelected();
    currentTest->draw();
  }
}

void bGroup::startTesting()
{
  bTesting=true;
  currentTest=0;
}

void bGroup::stopTesting()
{
  bTesting=false;
}

bool bGroup::isTesting()
{
  return bTesting;
}

void bGroup::pauseSequence()
{
  
}*/
