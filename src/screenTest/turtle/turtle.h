/*
 *  turtle.h
 *  robotBlocks
 *
 *  Created by Exhibits on 7/19/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofExtended.h"

class ofTurtle {
  int whlWid;
  int whlHgt;
  deque<ofPoint> lines;
  int numLineStored;
  bool bTurnSinceRecord;
  
  bool bCompleted;
  bool bCrashed;
  vector<ofVector> bdy;
  
  double frontCheckDist;
  double leftCheckDist;
public:
  ofImage map;
  ofPoint start,pos;
  int w,h;
  ofVector bearing;
  bool bMoving;
  ofTurtle();
  void setup(int _x, int _y, int _w, int _h);
  void setup(string filename);
  void move(int pixels);
  void turn(int degrees);
  void clear(){ lines.clear();}
  bool checkPoints();
  bool sensorIsClear(ofPoint strtPnt,int pixels, ofImage & walls, int direction=0);
  bool frontIsClear(int pixels, ofImage & walls);
  bool frontLeftIsClear(int pixels, ofImage & walls);
  bool frontRightIsClear(int pixels, ofImage & walls);
  bool rightIsClear(int pixels, ofImage & walls);
  bool leftIsClear(int pixels, ofImage & walls);
  bool frontIsClear(int distance);
  bool leftIsClear(int distance);
  bool crashed(){ return bCrashed;}
  bool completedMaze(){ return bCompleted;}
  ofPoint pointAlongBearing(int pix);
  void draw(int _x, int _y);
  void reset();
};