/*
 *  turtleAction.cpp
 *  arenaBot
 *
 *  Created by Exhibits on 11/11/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "turtleAction.h"
#include "../../blocks.h"

#include "../turtle/turtle.h"

#include "../evaluateExpression/evaluate.h"

extern int pixPerInch;

extern ofTimer actionTimer;

extern block * currentBlock;

turtleAction::turtleAction(block * prnt, ofTurtle * bdy)
{
  parentBlock=prnt;
  turtle=bdy;
  bParsed=bExecuted=false;
  bNegate=bData=false;
  guard=ACT_NORMAL;
  nCurrent=0;
  parse(parentBlock->origTag.getNode("action", 0).getValue());
  for (unsigned int i=0; i<parentBlock->blocksIn.size(); i++) {
    inside.push_back(turtleAction(&(parentBlock->blocksIn[i]), turtle));
  }
}

turtleAction::turtleAction(string act, block * prnt, ofTurtle * bdy)
{
  parentBlock=prnt;
  turtle=bdy;
  bParsed=bExecuted=false;
  bNegate=bData=false;
  guard=ACT_NORMAL;
  nCurrent=0;
  parse(act);
}

int turtleAction::direction()
{
  return 2*nGoal/abs(nGoal);
}

bool checkd=false;

bool turtleAction::assess()
{
  bool ret=false;
  switch (guard) {
    case ACT_NORMAL:
      if(abs(nCurrent)<abs(nGoal)){
        ret=true;
      }
      break;
    case LFT_SNS:
      if(turtle->leftIsClear(nGoal)) ret=true;
      break;
    case FRNT_SNS:
      if(turtle->frontIsClear(nGoal)) ret=true;
      break;
    case ACT_FOREVER:
      ret=true;
      break;
    default:
      break;
  }
  if(bNegate) ret=!ret;
  return ret;
}

void turtleAction::reset()
{
  bExecuted=false;
  nCurrent=0;
  //bData=false;
  resetGuardedActions();
}

bool turtleAction::executeGuardedAction()
{
  bool ret=0;
  for (unsigned int i=0; i<inside.size()&&!ret; i++) {
    ret=inside[i].execute();
  }
  return ret;
}

void turtleAction::resetGuardedActions()
{
  for (unsigned int i=0; i<inside.size(); i++) {
    inside[i].reset();
  }
}

bool turtleAction::execute()
{
  bool ret=false;
  if(!bExecuted){
    switch (type) {
      case TURTLE_MOVE:
        if(assess()){
          turtle->move(direction());
          nCurrent+=direction();
          currentBlock=parentBlock;
          ret=true;
          actionTimer.reset();
        }
        if(!assess()) bExecuted=true, ret=false,actionTimer.setPercent(.5);
        break;
      case TURTLE_TURN:
        if(assess()){
          turtle->turn(direction());
          nCurrent+=direction();
          currentBlock=parentBlock;
          ret=true;
          actionTimer.reset();
        }
        if(!assess()) bExecuted=true, ret=false,actionTimer.setPercent(.5);
        break;
      case TURTLE_WHILE:
        if(assess()){
          if(!(ret=executeGuardedAction()))
            resetGuardedActions(),ret=true;
        }
        else bExecuted=true;
        break;
        
        
      case TURTLE_IF:
        if(assess()||bData){
          bData=true;
          if(!(ret=executeGuardedAction()))
            bExecuted=true,bData=false,ret=true;
        }
        else bExecuted=true,bData=false;
        break;
        
        
      case TURTLE_REPEAT:
        if(assess()){
          if(!(ret=executeGuardedAction())){
            nCurrent++;
            resetGuardedActions(),ret=true;
          }
        }
        else bExecuted=true,resetGuardedActions();
        break;
      default:
        break;
    }
  }
  return ret;
}

void turtleAction::parse(string str)
{
  registerAction(str);
  parseAction();
  handleWord();
  bParsed=true;
}

void turtleAction::registerAction(string str)
{
  vector<string> insd=ofSplitString(str, "{}");
  if(insd.size()){
    vector<string> splt=ofSplitString(insd[0], ":");
    if(splt.size()>1){
      if(splt[0]=="move") type=TURTLE_MOVE;
      else if(splt[0]=="turn") type=TURTLE_TURN;
      else if(splt[0]=="while") type=TURTLE_WHILE;
      else if(splt[0]=="if") type=TURTLE_IF;
      else if(splt[0]=="repeat") type=TURTLE_REPEAT;
      dataStr=splt[1];
    }
    if(insd.size()>1) inside.push_back(turtleAction(insd[1],parentBlock,turtle));
  }
}

void turtleAction::handleWord()
{
  if(type==TURTLE_IF||type==TURTLE_WHILE||type==TURTLE_REPEAT){
    vector<string> spl=ofSplitString(dataStr, "$[]()");
    for (unsigned int i=0; i<spl.size(); i++) {
      if(spl[i]=="!") bNegate=true;
      else if(spl[i]=="front"){
        guard=FRNT_SNS;
        nGoal=ofToFloat(spl[i+1])*pixPerInch;
      }
      else if(spl[i]=="leftPath"){
        guard=LFT_SNS;
        nGoal=ofToFloat(spl[i+1])*pixPerInch;
      }
      else if(spl[i]=="forever") guard=ACT_FOREVER;
    }
  }
}

double turtleAction::evalVar(string str)
{
  double ret=0;
  vector<string> spl=ofSplitString(str, "$[]()");
  for (unsigned int i=0; i<spl.size(); i++) {
    if(spl[i]=="dd") ret=parentBlock->ddGroup[ofToInt(spl[i+1])].getValue();
    else if(spl[i]=="ppi") ret=pixPerInch;
  }
  return ret;
}

double turtleAction::parseNumber(string str)
{
  vector<evalData> eval=evaluateNumbers(dataStr);
  string t;
  for (unsigned int i=0; i<eval.size(); i++) {
    if(!eval[i].isNumber){
      eval[i].number=evalVar(eval[i].varName);
      eval[i].varName=ofToString(eval[i].number,0);
      eval[i].isNumber=true;
    }
    t+=eval[i].varName+eval[i].nextOperator;
  }
  eval=evaluateNumbers(t);
  return eval[0].number;
}

void turtleAction::parseAction()
{
  if(!bParsed){
    switch (type) {
      case TURTLE_MOVE:
        nGoal=parseNumber(dataStr);
        break;
      case TURTLE_TURN:
        nGoal=parseNumber(dataStr);
        break;
        /*case OF_BLOCK_WHILE:
         action.data[0]=parseNumber(action.dataStr);
         break;
         case OF_BLOCK_IF:
         action.data[0]=parseNumber(action.dataStr);
         break;*/
      case TURTLE_REPEAT:
        if(dataStr!="$forever")
          nGoal=parseNumber(dataStr);
        break;
      default:
        break;
    }
    bParsed=true;
  }
}