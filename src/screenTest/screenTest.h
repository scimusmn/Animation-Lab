/*
 *  screenTest.h
 *  arenaBot
 *
 *  Created by Exhibits on 11/11/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "turtle/turtle.h"
#include "turtleAction/turtleAction.h"

#include "../../../dallasEng/dallasEng.h"

class baseBlock;

enum ofBlockActions {
  OF_BLOCK_NULL_ACT,OF_BLOCK_MOVE, OF_BLOCK_TURN, OF_BLOCK_WHILE, OF_BLOCK_IF,OF_BLOCK_REPEAT
};

class robotTest {
protected:
  vector<turtleAction> actions;
  bool bTesting;
  bool bRunning;
  ofTurtle turtle;
  baseBlock * base;
  
  bool bFinished;
  
  dallasButton resetBut;
  dallasButton endBut;
  
  ofFont display;
public:
  ofRectangle controlBar;
  ofRectangle mapArea;
  
  void setup(baseBlock * t);
  
  void parseActions(baseBlock * t);
  
  void startTesting(baseBlock * t);
  
  void stopTesting();
  
  bool isTesting();
  
  void startTurtle();
  
  bool idleTurtle();
  
  void pauseTurtle();
  
  void resetTurtle();
  
  bool turtleIsRunning();
  
  bool turtleCrashed();
  
  bool finishedActions();
  
  void drawCurrentBlock();
  
  void draw(int x, int y, int w, int h);
  
  void drawMap(int x, int y, int w, int h);
  
  void drawControlBar(int x, int y);
  
  void drawForeground();
  
  bool mouseLockout();
  
  bool clickDown(int _x, int _y);
  
  bool clickUp();
};

robotTest & test();