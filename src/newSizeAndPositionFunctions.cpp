/*
 *  newSizeAndPositionFunctions.cpp
 *  robotBlocks
 *
 *  Created by Exhibits on 7/7/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */


#include "blocks.h"

int block::newHeightOn()
{
  int ret=0;
  for (unsigned int i=0; i<blocksOn.size(); i++) {
    blocksOn[i].newUpdateHeight();
    ret+=blocksOn[i].h;
  }
  return ret;
}

int block::newHeightIn()
{
  int ret=0;
  for (unsigned int i=0; i<blocksIn.size(); i++) {
    blocksIn[i].newUpdateHeight();
  }
  
}

int block::newUpdateHeight()
{
  int ret=0;
  int topHeight=ttlSize.y;
  if(type==BLK_BRACKET){
    for (unsigned int i=0; i<blocksIn.size(); i++) {
      ret+=blocksIn[i].newUpdateHeight();
    }
    //ret+=5;
    if(ret<orig.height-(ttlSize.y+bottomBar)) ret=orig.height-(ttlSize.y+bottomBar);
    interior.height=ret;
    h=ret+topHeight+bottomBar;
  }
  for (unsigned int i=0; i<blocksOn.size(); i++) {
    blocksOn[i].newUpdateHeight();
  }
  return h+insertSpace;
}

//********************* position boolean functions ********************

bool block::inside(block & drop)
{
  bool ret=0;
  if(type==BLK_BRACKET){
    int inLine=y+ttlSize.y/2;
    //int bottomSpace=orig.height-(interior.y+interior.height);
    int inH=h-((inLine-y)+bottomBar/2);
    if(drop.inBounds(x+interior.x, inLine, fullWidth()-interior.x, inH))
      ret=true;
  }
  return ret;
}

bool block::beneath(block & chk,signed int blw)
{
  if(blw<ttlSize.y/2-5){
    blw=ttlSize.y;
  }
  else blw+=ttlSize.y;
  int midLine=y+h-ttlSize.y/2;
  
  if(type==BLK_BRACKET){
    midLine=y+(interior.y+interior.height+bottomBar/2);
    blw=blw+(y+h)-midLine;
  }
  
  return (chk.inBounds(x, midLine, fullWidth(), blw));
}

bool block::inBounds(int xX, int yX, int wX, int hX)
{
  bool ret=0;
  int pH=h;
  if(type==BLK_BRACKET) pH=interior.y;
  if(((x>=xX && x<xX+wX)||(x+w>=xX && x+w<xX+wX)||(x<=xX && x+w >= xX + wX))&&((y>=yX && y<yX+hX)||(y+pH>=yX && y+pH<yX+hX))) ret=true;
  return ret;
}

int block::onBlockOn(int _x,int _y){
	//-------- if we are over a block below, and not looking for a dd click, returns the block's number in the vector+1.
	//-------- else, returns 0 (false);
	int ret=0;
	for (unsigned int i=0; i<blocksOn.size(); i++) {
		if(blocksOn[i].over(_x,_y)&&blocksOn[i].ddPassingClick(_x,_y)) ret=i+1;
	}
	return ret;
}

bool block::inOrOn(int _x, int _y)
{
  return (_x>x&&_x<x+fullWidth()&&_y>y&&_y<y+h+newHeightOn());
}

/*****************************************************************
 * fullWidth() :: function of block
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    NONE :
 *
 *  Output________
 *
 *    double :
 *
 */

double block::fullWidth()
{
  double ret=0;
  if(blocksIn.size()){
    ret+=blocksIn[0].x-x;
    double maximum=0;
    for (unsigned int i=0; i<blocksIn.size(); i++) {
      maximum=max(maximum,interior.x+blocksIn[i].fullWidth());
    }
    ret+=maximum;
  }
  ret=max(w,ret);
  return ret;
}

