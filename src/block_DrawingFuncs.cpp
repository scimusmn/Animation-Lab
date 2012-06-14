/*
 *  blocks_DrawingFuncs.cpp
 *  robotBlocksVectors
 *
 *  Created by Exhibits on 3/9/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "blocks.h"
#include "blockDraw.h"

extern ofColor white, gray, black, yellow, blue, red, orange;

//-------------------------block Draw Functions ---------------

void block::drawShadow(){
	//-------- if a block is a bracket block, draw a big shadow
	
	ofSetColor(0, 0, 0,75);
	if (type==BLK_BRACKET) {
		drawBigBlock(x+5,y+5,w,h,interior.x,interior.y,h-bottomBar);
	}
	//-------- if a block is a value block, draw the rounded block shadow
	else if(type==BLK_VAL) {
		if(bGrabbed) ofSetShadowDarkness(.4),ofShadowRounded(x+5,y+5,w,h,h/2);
	}
	//-------- if it's a normal block, draw a regular block shadow
	else {
		drawBlock(x+5,y+5,w,h);
	}
	//-------- draw shadows for blocks on
	for (unsigned int i=0; i<blocksOn.size(); i++) {
		blocksOn[i].drawShadow();
	}
	//-------- draw shadows for blocks inside
	for (unsigned int i=0; i<blocksIn.size(); i++) {
		blocksIn[i].drawShadow();
	}
}

void block::drawSelected(){
	//-------- if a block is a bracket block, draw a big shadow
	
	ofSetColor(yellow);
  ofNoFill();
  ofSetLineWidth(8);
	if (type==BLK_BRACKET) {
		drawBigBlock(x,y,w,h,interior.x,interior.y,h-bottomBar);
	}
	//-------- if a block is a value block, draw the rounded block shadow
	else if(type==BLK_VAL) {
		if(bGrabbed) ofSetShadowDarkness(.4),ofShadowRounded(x,y,w,h,h/2);
	}
	//-------- if it's a normal block, draw a regular block shadow
	else {
		drawBlock(x,y,w,h);
	}
	//-------- draw shadows for blocks on
	for (unsigned int i=0; i<blocksOn.size(); i++) {
		//blocksOn[i].drawSelected();
	}
	//-------- draw shadows for blocks inside
	for (unsigned int i=0; i<blocksIn.size(); i++) {
		//blocksIn[i].drawSelected();
	}
  ofSetLineWidth(2);
  ofFill();
}

void block::drawDD(){
	//-------- draw the dropdowns for blocks beneath
	for (unsigned int i=0; i<blocksOn.size(); i++) {
		blocksOn[i].drawDD();
	}
	
	//-------- draw the dropdowns for blocks inside
	for (unsigned int i=0; i<blocksIn.size(); i++) {
		blocksIn[i].drawDD();
	}
	
	//-------- draw the dd for the current block, if it is not open
	for (unsigned int i=0; i<ddGroup.size(); i++) {
		if(!ddGroup[i].open){
      ddGroup[i].draw(x+ddGroup[i].relPos.x,y+ddGroup[i].relPos.y);
		}
	}
}

void block::drawOpenDD(){
	//-------- draw dds for below and in
	for (unsigned int i=0; i<blocksOn.size(); i++) {
		blocksOn[i].drawOpenDD();
	}
	for (unsigned int i=0; i<blocksIn.size(); i++) {
		blocksIn[i].drawOpenDD();
	}
	
	//-------- draw open dd for current block
	for (unsigned int i=0; i<ddGroup.size(); i++) {
		if(ddGroup[i].open){
			ddGroup[i].draw(x+ddGroup[i].relPos.x,y+ddGroup[i].relPos.y);
		}
	}
}

void block::draw(int _x, int _y)
{
	x=_x;
	y=_y;
	draw();
}

void block::draw(bool fade){
  //-------- we set the color of the block and draw the shape depending on type
  ofSetColor(color.opacity(1));
  ofSetLineWidth(2);
  if (type==BLK_BRACKET){
    drawBigBlock(x,y,w,h,interior.x,interior.y,h-bottomBar);//drawBigBlock(x,y,w,h,orig.height,orig.width);
    ofSetColor(black);
    ofNoFill();
    drawBigBlock(x,y,w,h,interior.x,interior.y,h-bottomBar);
    ofFill();
  }
  else if(type==BLK_VAL) ofRaised(.3),ofRoundedRect(x,y,w,h,h/4);
  else {
    drawBlock(x,y,w,h);
    ofSetColor(black);
    ofNoFill();
    drawBlock(x,y,w,h);
    ofFill();
  }
  
  /*glColor3f(1,1,1);
  arialHeader.drawString(title,x+titlePos.x,y+titlePos.y);*/
  ofSetColor(white);
  int ttlPs=titlePos.x;
  for(unsigned int i=0; i<ttlSpl.size(); i++){
	  if(ttlSpl[i].txt!="%d"){
		  arialHeader.drawString(ttlSpl[i].txt,x+ttlPs,y+titlePos.y);
		  ttlPs+=ttlSpl[i].wid+spaceSize;
	  }
	  else ttlPs+=ttlSpl[i].wid+spaceSize;
  }
  
	//-------- for each of the statement positions,draw a placeholder or the block in that position
	for (unsigned int i=0; i<numBlocks.size(); i++) {
		if(numBlocks[i].placeHolder){
			ofSetColor(255, 255, 255);
			int Ho=((h>35)?35:h)-numBlocks[i].h;
      ofRaised(.3);
			ofRoundedRect(x+5+numBlocks[i].relPos.x,y+Ho/2,numBlocks[i].w,numBlocks[i].h,(20/4));
		}
		else {
			numBlocks[i].draw();
		}
	}
  //-------- draw the blocks inside
  int hIn=interior.y;
	for (unsigned int i=0; i<blocksIn.size(); i++) {
		blocksIn[i].draw(x+interior.x,y+hIn);
    hIn+=blocksIn[i].h;
	}
  if(hIn+bottomBar>=orig.height) h=hIn+bottomBar, interior.height=hIn-interior.y;
  else h=orig.height;
	//-------- Draw the blocks below
  int hOn=0;
	for (unsigned int i=0; i<blocksOn.size(); i++) {
		blocksOn[i].draw(x,y+h+hOn);
    hOn+=blocksOn[i].h;
	}
	
	//-------- draw all of the drop downs
	drawDD();
	
	drawOpenDD();
}


