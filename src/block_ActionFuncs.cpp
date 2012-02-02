/*
 *  blockActions.cpp
 *  robotBlocks
 *
 *  Created by Exhibits on 7/13/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

extern int pixPerInch;

#include "blocks.h"

struct evalData {
  bool isNumber;
  double number;
  string varName;
  int usrData;
  char nextOperator;
  evalData(double num){
    number=num;
    isNumber=true;
    usrData=nextOperator=0;
  }
  evalData(){
    isNumber=false;
    usrData=nextOperator=0;
  }
  evalData(string var){
    isNumber=false;
    varName=var;
    usrData=nextOperator=0;
  }
};

evalData operat(evalData & first, evalData & second)
{
  evalData ret;
  switch (first.nextOperator) {
    case '-':
      ret.number=first.number-second.number;
      break;
    case '+':
      ret.number=first.number+second.number;
      break;
    case '/':
      ret.number=first.number/second.number;
      break;
    case '*':
      ret.number=first.number*second.number;
      break;
    default:
      break;
  }
  ret.isNumber=true;
  ret.nextOperator=second.nextOperator;
  
  ret.varName=ofToString(ret.number,0);

  return ret;
}

int getNumDelim(string exp, string delims)
{
  int ret=0;
  for (unsigned int i=0; i<exp.length(); i++) {
    if (isDelim(exp[i],delims)) {
      ret++;
    }
  }
  return ret+1;
}

evalData getSubExp(string exp, int j)
{
  evalData ret;
  string delims="+-/*";
  string expr;
  int strt=0, end=0;
  int expNum=0;
  int numDeltd=getNumDelim(exp, delims);
  if(j>numDeltd) j=numDeltd;
  for (unsigned int i=0; i<exp.length(); i++) {
    if (isDelim(exp[i],delims)) {
      strt=((expNum==0)?end:end+1);
      end=i;
      if(expNum==j){
        expr=string(exp,strt,end-strt);
        if(i<exp.length()-1&&i) ret.nextOperator=exp[i];
        else if(i==0) ret.nextOperator=exp[i];
      }
      expNum++;
    }
  }
  if(j==expNum){
    strt=((j>0)?end+1:end);
    end=exp.length();
    expr=string(exp,strt,end-strt);
  }
  ret.varName=expr;
  if(expr.length()&&expr[0]!='$') ret.number=ofToInt(ret.varName), ret.isNumber=true;
  if(!expr.length()&&ret.nextOperator=='-'){
    ret.number=-1;
    ret.isNumber=true;
    ret.nextOperator='*';
    ret.varName="-1";
  }

  return ret;
}

vector<evalData> breakIntoEval(string exp)
{
  vector<evalData> ret;
  for (int i=0; i<getNumDelim(exp, "+-/*"); i++) {
    ret.push_back(getSubExp(exp, i));
  }
  return ret;
}

vector<evalData> evalNumbers(string exp)
{
  vector<evalData> ret;
  vector<evalData> eval=breakIntoEval(exp);
  for (unsigned int i=0; i<eval.size(); i++) {
    if(i<eval.size()-1&&eval[i].isNumber&&eval[i+1].isNumber){
      if((eval[i+1].nextOperator!='*'&&eval[i+1].nextOperator!='/')||(eval[i].nextOperator=='*'||eval[i].nextOperator=='/')){
        evalData newData=operat(eval[i], eval[i+1]);
        eval[i]=newData;
        eval.erase(eval.begin()+i+1);
      }
    }
  }
  for (unsigned int i=0; i<eval.size(); i++) {
    if(i<eval.size()-1&&eval[i].isNumber&&eval[i+1].isNumber){
      evalData newData=operat(eval[i], eval[i+1]);
      eval[i]=newData;
      eval.erase(eval.begin()+i+1);
    }
  }
  ret=eval;
  return ret;
}

void block::registerAction(string str)
{
  vector<string> splt=ofSplitString(str, ":");
  if(splt.size()>1){
    if(splt[0]=="move") action.act=OF_BLOCK_MOVE;
    else if(splt[0]=="turn") action.act=OF_BLOCK_TURN;
    else if(splt[0]=="while") action.act=OF_BLOCK_WHILE;
    else if(splt[0]=="if") action.act=OF_BLOCK_IF;
    else if(splt[0]=="repeat") action.act=OF_BLOCK_REPEAT;
    action.dataStr=splt[1];
  }
}

double block::evalVar(string str)
{
  double ret=0;
  vector<string> spl=ofSplitString(str, "$[]()");
  for (unsigned int i=0; i<spl.size(); i++) {
    if(spl[i]=="dd") ret=ddGroup[ofToInt(spl[i+1])].getValue();
    else if(spl[i]=="ppi") ret=pixPerInch;
  }
  return ret;
}

double block::parseNumber(string str)
{
  vector<evalData> eval=evalNumbers(action.dataStr);
  string t;
  for (unsigned int i=0; i<eval.size(); i++) {
    if(!eval[i].isNumber) eval[i].number=evalVar(eval[i].varName),eval[i].varName=ofToString(eval[i].number,0), eval[i].isNumber=true;
    t+=eval[i].varName+eval[i].nextOperator;
  }
  eval=evalNumbers(t);
  return eval[0].number;
}

void block::parseAction()
{
  if(!action.bParsed){
    switch (action.act) {
      case OF_BLOCK_MOVE:
        action.data[0]=parseNumber(action.dataStr);
        break;
      case OF_BLOCK_TURN:
        action.data[0]=parseNumber(action.dataStr);
        break;
      /*case OF_BLOCK_WHILE:
        action.data[0]=parseNumber(action.dataStr);
        break;
      case OF_BLOCK_IF:
        action.data[0]=parseNumber(action.dataStr);
        break;*/
      case OF_BLOCK_REPEAT:
        if(action.dataStr!="$forever")
          action.data[0]=parseNumber(action.dataStr);
        break;
      default:
        break;
    }
    action.bParsed=true;
  }
  for (unsigned int i=0; i<blocksIn.size(); i++) {
    blocksIn[i].parseAction();
  }
  for (unsigned int i=0; i<blocksOn.size(); i++) {
    blocksOn[i].parseAction();
  }
}

void block::executeAction()
{
  
}

