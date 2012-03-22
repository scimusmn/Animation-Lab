/*
 *  progressReader.h
 *  arenaBot
 *
 *  Created by Exhibits on 11/29/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofExtended.h"
#include "ofxSystemCall.h"

class progRead : public ofxThread{
protected:
  ofTimer checkTimer;
  double percent;
  ofSystemCall sys;
  string message;
  int fd;
  string rep;
  int lineCount;
public:
  progRead();
  ~progRead();
  double percentDone();
  string report();
  void checkFile();
  void reset();
  void start();
  
  void stop();
  
  //--------------------------
  void threadedFunction();
};