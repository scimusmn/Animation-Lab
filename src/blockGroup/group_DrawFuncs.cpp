/*
 *  groupDrawFuncs.cpp
 *  arenaBot
 *
 *  Created by Exhibits on 11/1/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "blockGroup.h"
#include "../robotConfig.h"

extern ofColor yellow,red,black,white,blue,gray;

//---------------Group Draw Functions ------------------

/*****************************************************************
 * indicate(dropBlock & db) 
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    dropBlock & db :
 *
 *  Output________
 *
 *    NONE :
 *
 */


void indicate(dropBlock & db)
{
  ofSetColor(255, 255, 255);
  if(db.index){
    block & t=(*db.inThis)[db.index-1];
    ofRect(t.x+5, t.y+t.h, t.w-10, 10);
  }
  else {
    block & t=*(db.belowThis);
    if(db.whichVector==OF_BLOCK_IN)
      ofRect(t.x+t.interior.x+5, t.y+t.interior.y, t.w-t.interior.x-10, 10);
    else
      ofRect(t.x+5, t.y+t.h, t.w-10, 10);
  }
}

bool bGroup::drawIndicators(block & held, block & k)
{
  bool ret=0;
  dropBlock db=underWhich(k, held);
  if(db.found()){
    indicate(db);
    ret=1;
  }
  if(!db.found()){
    db=underWhich(held, k);
    if(db.found()&&db.index==db.inThis->size()){
      indicate(db);
      ret=1;
    }
  }
  return ret;
}

void bGroup::draw(int _x, int _y, int _w, int _h)
{
  x=_x,y=_y,w=_w,h=_h;
  draw();
  base.draw(x+(w-base.w)/2, y-bar.getScrollPosition());
  base.drawButtonArea(x+(w-base.w)/2, y);
  ofSetColor(black.opacity(.5));
  if(bScrolling){
    bar.draw(x+w-bar.w,y);
    if(bar.getScrollPosition()>50){
      ofSetColor(yellow);
      if(!cfg().portraitMode) label.drawString("Program continues above\nuse scrollbar to view",base.x+ base.w+20, y+50);
	  else label.drawString("Program continues above\nuse scrollbar to view",base.x+ base.w-base.butArea.x, y+base.h+50);
    }
    if(bar.getScrollPosition()<bar.getFullSize()-(h+200)&&bar.available()){
      ofSetColor(yellow);
      if(!cfg().portraitMode) label.drawString("Program continues below\nuse scrollbar to view",base.x+ base.w+20, y+h-label.stringHeight("Program\nuse")-20);
	  else label.drawString("Program continues below\nuse scrollbar to view",base.x+ base.w-base.butArea.x, y+h-label.stringHeight("Program\nuse")-20);
    }
  }
}

void bGroup::drawBase(int _x, int _y)
{
	//base.drawSelected();
  y=_y;
	base.draw(_x,_y);
}

void bGroup::draw(){
	//-------- if the blocks are not grabbed, and the dds are not open, draw the blocks
	for (unsigned int i=0; i<size(); i++) {
		if (!blocks[i].bGrabbed&&!blocks[i].ddOpen) {
			blocks[i].draw();
		}
	}
}

void bGroup::drawForeground(){
	//******** if a block is currently being held, draw it on different layers, depending on Circumstances
	//-------- draw all the shadows
	for (unsigned int i=0; i<blocks.size(); i++) {
		if (blocks[i].bGrabbed) {
			blocks[i].drawShadow();
		}
	}
	//-------- draw all the grabbed blocks
	for (unsigned int i=0; i<blocks.size(); i++) {
		if (blocks[i].bGrabbed) {
			blocks[i].draw();
		}
	}
	
	//-------- draw ungrabbed statement blocks
	for (unsigned int i=0; i<blocks.size(); i++) {
		if (blocks[i].type==BLK_VAL&&!blocks[i].bGrabbed) {
			blocks[i].draw();
		}
	}
	//-------- draw grabbed statement blocks
	for (unsigned int i=0; i<blocks.size(); i++) {
		if (blocks[i].type==BLK_VAL&&blocks[i].bGrabbed) {
			blocks[i].draw();
		}
	}
	
	//-------- Not deprecated, displays blocks when dd clicked.
	for (unsigned int i=0; i<blocks.size(); i++) {
		if (blocks[i].ddOpen) {
			blocks[i].draw();
		}
	}
  
  if(held.bGrabbed){
    held.drawShadow();
    held.draw();
  }
  
  if(inHand&&held.bGrabbed){
    bool drawnInd=0;
    drawnInd=drawIndicators(held, base);
    for (unsigned int i=0; i<size()&&!drawnInd; i++) {
      drawnInd=drawIndicators(held,blocks[i]);
    }
  }
  
  
}