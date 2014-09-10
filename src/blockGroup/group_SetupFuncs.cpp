/*
 *  blockGroup.cpp
 *  arenaBot
 *
 *  Created by Exhibits on 11/1/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "blockGroup.h"


extern int pixPerInch;

//static bGroup blcks;

//bGroup & blocks(){
//	return blcks;
//}

/****************************************************************
 *********************BlockGroup*********************************/

bGroup::bGroup(double _x, double _y,double wid,double hgt):ofInterObj(_x,_y,wid,hgt){
	//blocks.reserve(100);
	x=_x,y=_y,w=wid,h=hgt;
	used[""]=false;
}

/*****************************************************************
 * bGroup():ofInterObj() :: constructor for bGroup, a subclass of ofInterObj()
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
 *    New instance of bGroup :
 *
 */

bGroup::bGroup():ofInterObj(){
	bGrabbed=inHand=ddopen=0;
}

bGroup::~bGroup(){
	blocks.clear();
}

//------------------------ Utilities -------------------

void bGroup::setScrolling(bool scrl)
{
  bScrolling=scrl;
}

void bGroup::setup(double _x, double _y,double wid,double hgt){
	//blocks.reserve(100);
  bSequencePlay=bTesting=false;
	bGrabbed=inHand=ddopen=bChanged=false;
	cSetup(_x,_y,wid,hgt);
	used[""]=false;
  base.setup(this);
	base.blocksOn.reserve(100);
	//states.recordState(storageState(blocks,base));
  held.setup(0,0);
  
  bar.setup(60, h, OF_VERT);
  bar.registerArea(h,base.newHeightOn()+base.h);
  bar.changePadding();
  bar.setScrollPosition(0);
  
  current=0;
  
  label.loadFont("fonts/Din.ttf");
  label.setSize(19);
  
  bScrolling=true;
  saveStateAvailable=false;
}

int bGroup::size(){
	return blocks.size();
}

void bGroup::clear(){
	base.blocksOn.clear();
	blocks.clear();
}

block bGroup::operator[](int i){
	return blocks[i];
}

void bGroup::recordState()
{
	states.recordState(storageState(blocks,base));
}

void bGroup::loadState()
{
	if(saveStateAvailable){
		blocks=save_state.blocks;
		base=save_state.base;
	}
}

bool bGroup::stateAvailable()
{
	return saveStateAvailable;
}

void bGroup::saveState()
{
	saveStateAvailable=true;
	save_state.blocks=blocks;
	save_state.base=base;
}

void bGroup::undoState()
{
	if(states.undoAvailable()){
    storageState * t;
    if((t=states.undoState())){
      blocks=t->blocks;
      base=t->base;
    }
  }
}

void bGroup::redoState()
{
	if(states.redoAvailable()){
    storageState * t;
    if((t=states.redoState())){
      blocks=t->blocks;
      base=t->base;
    }
  }
}

bool bGroup::undoAvailable()
{
	return states.undoAvailable();
}

bool bGroup::redoAvailable()
{
	return states.redoAvailable();
}


//----------------------- Add block Functions -------------------

int bGroup::totalBlocks()
{
  int ret=0;
  for (unsigned int i=0; i<base.blocksOn.size(); i++) {
      if(base.blocksOn[i].type==BLK_BRACKET)
         ret+=1+base.blocksOn[i].totalInside();
  }
  for (unsigned int i=0; i<blocks.size(); i++) {
    ret++;
    for (unsigned int j=0; j<blocks[i].blocksOn.size(); j++) {
        if(base.blocksOn[i].type==BLK_BRACKET)
            ret+=1+base.blocksOn[j].totalInside();
    }
  }
  return ret;
}

void bGroup::addFromSB(block t,int _x,int _y){
	if(!inHand){
		if(totalBlocks()<50){
			used[t.label]=false;
            used[t.classLabel]=false;
      held=t;
      held.bGrabbed=bGrabbed=inHand=true;
			dispx = held.x-_x;
			dispy = held.y-_y;
		}
		else {
			printf("Remove some blocks, doncha know.");
		}
	}
}

void bGroup::clearBlocksByLabel(string label){
  base.removeBlocksByLabel(label);
  for (unsigned int i=0; i<blocks.size(); i++) {
    blocks[i].removeBlocksByLabel(label);
    removeFromVector(blocks,label);
  }
}

void bGroup::clearAndReset()
{
	blocks.clear();
	base.blocksOn.clear();
	current=0;
	ddopen=false;
	inHand=false;
}

void bGroup::update()
{
	//bar.update();
}

void bGroup::resize(int _x, int _y, int _w ,int _h){
	h=_h;
  bar.setup(60, h, OF_VERT);
  bar.registerArea(h,base.newHeightOn()+base.h+200);
  bar.changePadding();
}

void bGroup::setFocus(block * cur){
  current = cur;
}

void bGroup::loseFocus(){
  current=0;
}
