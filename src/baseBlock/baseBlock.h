/*
 *  baseBlock.h
 *  arenaBot
 *
 *  Created by Exhibits on 11/9/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#pragma once

#include "../blocks.h"

class bGroup;

class baseBlock : public block {
protected:
  bGroup * group;
  int pad;
  bool bDrawtest;
public:
  ofPoint butArea;
  dallasButton uploadBut;
  dallasButton testBut;
  baseBlock();
  baseBlock & operator=(baseBlock &t);
  void draw(int _x, int _y);
  void drawButtonArea(int _x, int _y);
  bool clickDown(int _x, int _y);
  bool newClickUp( int _x, int _y);
  void setup(bGroup * grp);
  void setDrawTest(bool bDraw);
  bool beneath(block & blockToCheck,signed int spaceBelow=-1);
  int widthOn();
};