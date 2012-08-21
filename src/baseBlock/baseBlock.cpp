/*
 *  baseBlock.cpp
 *  arenaBot
 *
 *  Created by Exhibits on 11/9/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "baseBlock.h"
#include "../blockDraw.h"
#include "../robotConfig.h"

extern ofColor white, black, blue, yellow, gray;

extern int TITLE_HEIGHT;

void baseBlock::setup(bGroup * grp)
{
  group=grp;
  bDrawtest=true;
  w=575, h=90;
  orig.width=w;
  orig.height=h;
  interior.height=bottomBar=0;
  interior.y=0;
  interior.x=0;
	arialHeader.loadFont("fonts/DinC.ttf");
	arialHeader.setSize(cfg().blockFontSize);
  arialHeader.setMode(OF_FONT_TOP);
  //arialHeader.setMode(OF_FONT_MID);

  testBut.setup("Test program", cfg().buttonFontSize);
  uploadBut.setup("Upload program",cfg().buttonFontSize);
  pad=15;
  
  butArea.x=max(uploadBut.w,testBut.w)+pad*2;
  butArea.y=pad+uploadBut.h+pad+testBut.h+pad;
  
  title="Drag and drop blocks here";
  type=BLK_BASE;
	bGrabbed=0;
	w=max(w,double(arialHeader.stringWidth(title)+20)+butArea.x);
	ddOpen=false;
	titlePos.x=10;
	color=ofColor(yellow);
	filename="null";
	placeHolder=false;
  
  ttlSize.y=TITLE_HEIGHT;
  ttlSize.x=w;
  
  //uploadBut.setup( 128,128,"images/upload2.png");
  
  
  //else butArea.y=pad+uploadBut.h+pad;//+testBut.h+pad;
}

baseBlock::baseBlock():block(){
  
}

void baseBlock::setDrawTest(bool btest){
  //bDrawtest=btest;
}

void baseBlock::drawButtonArea(int _x, int _y)
{
  if(cfg().defaultColor) ofSetColor(black);
  else ofSetColor(cfg().subtitleColor);
  ofBeginShape();
  if(cfg().test) drawButtonSpace(x+w-butArea.x, _y, butArea.x, butArea.y,butArea.y-h);
  else drawButtonSpace(x+w-butArea.x, _y, butArea.x, h,0);
  ofEndShape();
  
  
  if(cfg().defaultColor) ofSetColor(yellow);
  else ofSetColor(cfg().lineColor);
  ofNoFill();
  ofSetLineWidth(2);
  ofBeginShape();
  if(cfg().test) drawButtonSpace(x+w-butArea.x, _y, butArea.x, butArea.y,butArea.y-h);
  else drawButtonSpace(x+w-butArea.x, _y, butArea.x, h,0);
  ofEndShape();
  
  ofSetLineWidth(1);
  ofFill();

  uploadBut.setTextSize(cfg().buttonFontSize);
  
  if(cfg().test){
	uploadBut.draw(x+w-butArea.x+(butArea.x-uploadBut.w)/2,_y+pad);
	testBut.draw(x+w-butArea.x+(butArea.x-testBut.w)/2, uploadBut.y+uploadBut.h+pad);
  }
  else uploadBut.draw(x+w-butArea.x+(butArea.x-uploadBut.w)/2,_y+(h-uploadBut.h)/2);

}

void baseBlock::draw(int _x, int _y)
{
  x=_x, y=_y;
  
  //w=max(ttlSize.x, widthOn()+butArea.x);
  
  if(cfg().defaultColor) ofSetColor(black);
  else ofSetColor(cfg().subtitleColor);
  drawBaseBlock(x, y, w, h,0,h);
  
  //-------- Draw the blocks below
  int hOn=0;
	for (unsigned int i=0; i<blocksOn.size(); i++) {
		blocksOn[i].draw(x,y+h+hOn);
    hOn+=blocksOn[i].h;
	}
	//-------- draw the blocks inside
	for (unsigned int i=0; i<blocksIn.size(); i++) {
		blocksIn[i].draw();
	}
  
  if(!bDrawtest) title="Program being tested";
  else title="Drag and drop blocks here";
  
  if(cfg().defaultColor) ofSetColor(yellow);
  else ofSetColor(cfg().textColor);
  arialHeader.drawString(title, x+10, y+(h-arialHeader.stringHeight(title))/2);

  if(cfg().defaultColor) ofSetColor(yellow);
  else ofSetColor(cfg().lineColor);
  ofNoFill();
  ofSetLineWidth(2);
  drawBaseBlock(x, y, w, h,0,h);
  ofSetLineWidth(1);
  ofFill();

  if(!blocksOn.size()){
	if((ofGetElapsedTimeMillis()%1000)<500)
		ofSetColor(blue.opacity((ofGetElapsedTimeMillis()%500)/500.));
	else ofSetColor(blue.opacity(1-(ofGetElapsedTimeMillis()%500)/500.));
	//drawBaseBlock(x, y, w, h,0,h);
	int sz=TITLE_HEIGHT/4;
	ofRect(x+10,y+3*h/4-10,w-20-butArea.x,10);
	ofRect(x+3*sz-4,y+3*h/4,8,h/4-10);

	ofTriangle(x+2*sz-10,y+h-10,x+3*sz,y+h+sz,x+4*sz+10,y+h-10);
  }
  
  drawOpenDD();
}

bool baseBlock::clickDown(int _x, int _y)
{
  return uploadBut.clickDown(_x, _y);
}

bool baseBlock::newClickUp( int _x, int _y)
{
  return uploadBut.clickUp();
}

bool baseBlock::beneath(block & chk,signed int blw)
{
  if(blw<ttlSize.y/2-5&&blocksOn.size()){
    blw=ttlSize.y;
  }
  else if(blw<ttlSize.y/2-5&&!blocksOn.size()){
	  blw=75;
  }
  else blw+=ttlSize.y;
  int midLine=y+h-ttlSize.y/2;
  
  if(type==BLK_BRACKET){
    midLine=y+(interior.y+interior.height+bottomBar/2);
    blw=blw+(y+h)-midLine;
  }
  
  return (chk.inBounds(x, midLine, fullWidth(), blw));
}


baseBlock & baseBlock::operator=(baseBlock & t)
{
  x=t.x;
	y=t.y;
	w=t.w;
	h=t.h;
  relPos=t.relPos;
  
  orig=t.orig;
  interior=t.interior;
  titlePos=t.titlePos;
  ttlSize=t.ttlSize;
  
	arialHeader=t.arialHeader;
	type=t.type;
	bGrabbed=t.bGrabbed;
	title =t.title;
	filename=t.filename;
	numBlocks=t.numBlocks;
	
	ddGroup=t.ddGroup;
	blocksOn=t.blocksOn;
	blocksIn=t.blocksIn;
	ddOpen=t.ddOpen;
	placeHolder=t.placeHolder;
	color=t.color;
  insertSpace=t.insertSpace;
  
  return *this;
}

int baseBlock::widthOn(){
  double ret=0;
  for (unsigned int i=0; i<blocksOn.size(); i++) {
    ret=max(blocksOn[i].fullWidth(),ret);
  }
  return ret;
}