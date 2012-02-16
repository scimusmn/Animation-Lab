/*
 *  evaluate.cpp
 *  arenaBot
 *
 *  Created by Exhibits on 11/11/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "evaluate.h"

evalData operate(evalData & first, evalData & second)
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

int getNumDelimited(string exp, string delims)
{
  int ret=0;
  for (unsigned int i=0; i<exp.length(); i++) {
    if (isDelim(exp[i],delims)) {
      ret++;
    }
  }
  return ret+1;
}

evalData getSubExpression(string exp, int j)
{
  evalData ret;
  string delims="+-/*";
  string expr;
  int strt=0, end=0;
  int expNum=0;
  int numDeltd=getNumDelimited(exp, delims);
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

vector<evalData> breakIntoEvaluable(string exp)
{
  vector<evalData> ret;
  for (int i=0; i<getNumDelimited(exp, "+-/*"); i++) {
    ret.push_back(getSubExpression(exp, i));
  }
  return ret;
}

vector<evalData> evaluateNumbers(string exp)
{
  vector<evalData> ret;
  vector<evalData> eval=breakIntoEvaluable(exp);
  for (unsigned int i=0; i<eval.size(); i++) {
    if(i<eval.size()-1&&eval[i].isNumber&&eval[i+1].isNumber){
      if((eval[i+1].nextOperator!='*'&&eval[i+1].nextOperator!='/')||(eval[i].nextOperator=='*'||eval[i].nextOperator=='/')){
        evalData newData=operate(eval[i], eval[i+1]);
        eval[i]=newData;
        eval.erase(eval.begin()+i+1);
      }
    }
  }
  for (unsigned int i=0; i<eval.size(); i++) {
    if(i<eval.size()-1&&eval[i].isNumber&&eval[i+1].isNumber){
      evalData newData=operate(eval[i], eval[i+1]);
      eval[i]=newData;
      eval.erase(eval.begin()+i+1);
    }
  }
  ret=eval;
  return ret;
}