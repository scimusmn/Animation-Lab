/*
 *  groupMouseFuncts.cpp
 *  arenaBot
 *
 *  Created by Exhibits on 11/1/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */


#include "blockGroup.h"

extern bool bCopy;

/*************************** Mouse Functions ***************************
 ***********************************************************************/



void bGroup::motion(double _x, double _y){
	//-------- if we are moving but not clicked, inform the blocks
	for (unsigned int i=0; i<blocks.size(); i++) {
		blocks[i].motion(x,y);
	}
}

bool bGroup::clickDown(int _x, int _y)
{
  bool ret=false;
  if(current){
    if(current->newClickDD(_x, _y, ddopen)) ret=1;
    loseFocus();
  }
  else{
    ret=bar.clickDown(_x, _y);
    for (unsigned int i=0; i<blocks.size()&&!ret; i++) {
      if(!ret) ret=newHandleClick(blocks,i,_x,_y,true);
    }
    for (unsigned int i=0; i<base.blocksOn.size()&&!ret; i++) {
      if(!ret) 
        if(ret=newHandleClick(base.blocksOn,i,_x,_y)) bChanged=true;
    }
    if(!ret) ret=base.clickDown(_x, _y);
  }
  return ret;
}

bool bGroup::newHandleClick(vector<block> & chk, int i, int _x, int _y, bool top)
{
  bool ret=false;
  if(!chk[i].ddPassingClick(_x,_y)){
    setFocus(&chk[i]);
    if(chk[i].newClickDD(_x,_y,ddopen))
      ret=true;
  }
  else if(!ret&&chk[i].clickDown(_x,_y)&&!ddopen){
    inHand=ret=true;
    dispx = chk[i].x-_x;
    dispy = chk[i].y-_y;
    if(!top) pullBlocks(chk, i);
    else if(bCopy) held = chk[i];
    else held = chk[i],chk.erase(chk.begin()+i);
  }
  else if(chk[i].inOrOn(_x,_y)){
    for (unsigned int j=0; j<chk[i].blocksIn.size()&&!ret; j++) {
      if(!ret) ret=newHandleClick(chk[i].blocksIn,j,_x,_y,false);
    }
    for (unsigned int j=0; j<chk[i].blocksOn.size()&&!ret; j++) {
      if(!ret)ret=newHandleClick(chk[i].blocksOn,j,_x,_y,false);
    }
  }
  return ret;
}

void bGroup::pullBlocks(vector<block> & chk, int i)
{
  held=chk[i];
  held.blocksOn.assign(chk.begin()+(i+1),chk.end());
  if(!bCopy) chk.erase(chk.begin()+i,chk.end());
}

//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_

void resetInsertSpace(block & t){
  t.insertSpace=0;
  for (unsigned int i=0; i<t.blocksIn.size(); i++) {
    resetInsertSpace(t.blocksIn[i]);
  }
  for (unsigned int i=0; i<t.blocksOn.size(); i++) {
    resetInsertSpace(t.blocksOn[i]);
  }
}

void bGroup::drag(double _x, double _y){
	//-------- if we're holding a block, update the position
	if(inHand){
    if(held.bGrabbed){
      held.move(_x+dispx, _y+dispy);
    }
	}
  bar.mouseMotion(_x, _y);
}

bool bGroup::newClickUp(int _x, int _y)
{
  for (unsigned int i=0; i<blocks.size(); i++) {
    blocks[i].newClickUp(x,y);
    resetInsertSpace(blocks[i]);
  }
  base.newClickUp(x, y);
  bool ret=0;
  if(held.bGrabbed){
    held.newClickUp(_x,_y);
    if(!(held.x<x||held.x>x+w||held.y<y||held.y>y+h)){
      if(processBlockDrop(held, base)) ret=true,bChanged=true;
      for (unsigned int i=0; i<blocks.size()&&!ret; i++){
        ret=processBlockDrop(held, blocks[i]);
        if(!ret && (ret=processBlockDrop(blocks[i],held,true)))
          blocks.erase(blocks.begin()+i),i--;
      }
      if(!ret) pushBlocks(held, blocks, blocks.size(),true);
    }
    recordState();
  }
  bGrabbed=inHand=false;
  if(base.newHeightOn()+base.h+200!=bar.getFullSize()){
    float perc=bar.getScrollPercent();
    bar.setup(60, h, OF_VERT);
    bar.registerArea(h,base.newHeightOn()+base.h+200);
    bar.changePadding();
    bar.setScrollPercent(perc);
  }
  if(base.widthOn()+base.butArea.x!=base.w){
    //base.w=base.widthOn()+base.butArea.x;
  }
  bar.clickUp();
  return ret;
}

bool bGroup::processBlockDrop(block & drop,block & target,bool reverse)
{
  bool ret=0;
  dropBlock fnd;
  fnd=underWhich(target, drop);
  if(fnd.found()){
    if(!reverse){
      ret=true;
      pushBlocks(drop, *fnd.inThis, fnd.index);
    }
    else if(fnd.index==fnd.inThis->size()){
      pushBlocks(drop, *fnd.inThis, fnd.index);
      int pos=blocks.size();
      pushBlocks(target, blocks, pos,true);
      ret=true;
    }
  }
  return ret;
}

bool bGroup::pushBlocks(block & dropped, vector<block> & into, int i, bool top)
{
  dropped.bGrabbed=false;
  if(top) into.insert(into.begin()+i,dropped);
  else{
    vector<block> k=dropped.blocksOn;
    dropped.blocksOn.clear();
    into.insert(into.begin()+i,dropped);
    if(k.size()) into.insert(into.begin()+i+1,k.begin(),k.end());
  }
}
