/*
 *  evaluate.h
 *  arenaBot
 *
 *  Created by Exhibits on 11/11/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofExtended.h"

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

class expression {
protected:
  string expString;
  vector<evalData> expVec;
public:
  expression(string exp);
  void evaluate();
};

evalData operate(evalData & first, evalData & second);

int getNumDelimited(string exp, string delims);

evalData getSubExpression(string exp, int j);

vector<evalData> breakIntoEvaluable(string exp);

vector<evalData> evaluateNumbers(string exp);