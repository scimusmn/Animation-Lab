/*
 *  sideBar.cpp
 *  robotBlocksVectors
 *
 *  Created by Exhibits on 3/10/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "sideBar.h"

extern string defaultFont;

extern ofColor white, black, gray, yellow, blue, red, orange;

/****************************** Sidebar *******************************
 **********************************************************************/

/*****************************************************************
 * sideBar(int _x,int _y,int w,int h,string file,ofColor col):ofInterObj(_x,_y,w,h) :: constructor for sideBar, a subclass of ofInterObj(_x,_y,w,h)
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    _x :
 *    _y :
 *    w :
 *    h :
 *
 *  Output________
 *
 *    New instance of sideBar :
 *
 */

sideBar::sideBar(int _x,int _y,int w,int h,string file,ofColor col):ofInterObj(_x,_y,w,h){
	arialHeader.loadFont("fonts/HelveticaBold.otf");
	arialHeader.setSize(20);
	bOver=bPressed=false;
	bOpen = false;
	xo=_x;
	yo=_y;
	color=col;
	filename=file;
}

sideBar::sideBar(string title,ofColor col):ofInterObj(){
	arialHeader.loadFont("fonts/HelveticaBold.otf");
	arialHeader.setSize(20);
	bOver=bPressed=false;
	bOpen = false;
	color=col.opacity(1);
  h=arialHeader.stringHeight("Kjhg")*1.3;
	filename=title;
}

/*****************************************************************
 * sideBar():ofInterObj() :: constructor for sideBar, a subclass of ofInterObj()
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
 *    New instance of sideBar :
 *
 */

sideBar::sideBar():ofInterObj(){
	arialHeader.loadFont(defaultFont);
	arialHeader.setSize(20);
	bOver=bPressed=false;
	bOpen = false;
}

/*****************************************************************
 * ~sideBar() :: deconstructor for sideBar
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
 *    New instance of sideBar :
 *
 */

sideBar::~sideBar() {
}

/*****************************************************************
 * operator=(const sideBar t) :: function of sideBar
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    const sideBar t :
 *
 *  Output________
 *
 *    NONE :
 *
 */

void sideBar::operator=(const sideBar t) {
	x=t.x;
	y=t.y;
	w=t.w;
	h=t.h;
  color=t.color;
  filename=t.filename;
}

/*****************************************************************
 * draw(int _x, int _y) :: function of sideBar
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    int _x :
 *     int _y :
 *
 *  Output________
 *
 *    NONE :
 *
 */

void sideBar::draw(int _x, int _y){
	x=_x, y=_y;
	
  ofSetColor(gray);
  trimmedRect(x, y, w, h);
  ofSetColor(color);
	if(!bOpen) trimmedRect(x,y,w/8,h);
	else trimmedRect(x,y,w,h);
	ofSetColor(yellow);
  ofNoFill();
  trimmedRect(x,y,w,h);
  ofFill();
  ofSetColor(white);
	arialHeader.drawString(filename,x+w/8+10,y+3*h/4);
  int pad=20;
	if(bOpen){
		int temp=y+h+pad;
		for (unsigned int j=0; j<blocks.size(); j++) {
			blocks[j].draw(x+pad,temp);
      if(j<blocks.size()-1){
        //ofShade(x, temp+blocks[j].h+blocks[j].newHeightOn()+pad/2, 3, w-h-1, OF_UP);
        //ofShade(x, temp+blocks[j].h+blocks[j].newHeightOn()+pad/2, 3, w-h-1, OF_DOWN,false);
        ofSetColor(black.opacity(.5));
        ofRect(x, temp+blocks[j].h+blocks[j].newHeightOn()+pad/2, w, 1);
      }
			temp+=blocks[j].h+blocks[j].newHeightOn()+pad;
		}
	}
}

/*****************************************************************
 * updateSize() :: function of sideBar
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

double sideBar::updateSize()
{
	for (unsigned int i=0; i<blocks.size(); i++) {
		if (blocks[i].w>3*w/4) {
			w=max(w,blocks[i].w+40);
		}
	}
	return w;
}

/****************************** Sidebar Group *************************
 **********************************************************************/

/*****************************************************************
 * updateBlocks(int i) :: function of sbGroup
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    int i :
 *
 *  Output________
 *
 *    NONE :
 *
 */

void sbGroup::updateBlocks(int i){
	int divs=bars[i].blocks.size();
	for (unsigned int j=0; j<divs; j++) {
		int w=bars[i].blocks[j].w;
		bars[i].blocks[j].x=(bars[i].w-30-w)/2;
		bars[i].blocks[j].y=bars[i].y+5*bars[i].h/4+sideBarSpace*j*\
		((bars[i+1].y-(bars[i].y+bars[i].h))/sideBarSpace)/divs;
		bars[i].blocks[j].h=bars[i].blocks[j].orig.height*((bars[i+1].y-(bars[i].y+bars[i].h))/sideBarSpace);
	}
}


void sbGroup::setup(ofXML & xml,bGroup * destin)
{
  dest=destin;
  xml.setCurrentTag(";blocks");
	string font=xml.getCurrentTag().getAttribute("font");
	ofTag & tag=xml.getCurrentTag();
	for (unsigned int i=0; i<tag.size(); i++) {
		if (tag[i].getLabel()=="bar") {
			ofColor color=ofColor(strtol(tag[i].getAttribute("color").c_str(),NULL,0));
      //printf("color %i, %i, %i", int(color.r), int(color.g), int(color.b));
			unsigned int curBar=bars.size();
			bars.push_back( sideBar(tag[i].getAttribute("name"),color));
			for (unsigned int j=0; j<tag[i].size(); j++) {
				if (tag[i][j].getLabel()=="block") {
          int curBlock=bars[curBar].blocks.size();
					bars[curBar].blocks.push_back(block(tag[i][j],color));
          bars[curBar].w=max(bars[curBar].w,bars[curBar].blocks[curBlock].fullWidth());
          w=max(bars[curBar].w,w);
				}
			}
		}
	}
	bars.push_back( sideBar("Filler",ofColor(0,0,0)));
	if (bars.size()) {
		y=bars[0].y;
		x=0;
    barHeight=bars[0].h;
	}
  for (unsigned int i=0; i<bars.size(); i++) {
    bars[i].w=w;
  }
	updateHeight();
	if(bars.size()>=2) bars[0].bOpen=true;
}

void sbGroup::clear()
{
  bars.clear();
}

/*****************************************************************
 * setDest(bGroup * destin) :: member of sbGroup
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    bGroup Desktop Documents Downloads Java Library Mail Movies Music Pictures Public Sites bin doc example gmon.out include libtool modkit-community-edition-read-only python tmp xstc destin :
 *
 *  Output________
 *
 *    NONE :
 *
 */

void sbGroup::setDest(bGroup * destin){
	dest=destin;
}

/*****************************************************************
 * updateHeight() :: member of sbGroup
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
 *    NONE :
 *
 */

void sbGroup::updateHeight(){
	double maxWid=0;
	double maxHeight=0;
	for (unsigned int i=0; i<bars.size(); i++) {
		double hgt=20;
		for (unsigned int j=0; j<bars[i].size(); j++) {
			hgt+=bars[i][j].h+bars[i][j].newHeightOn()+20;
		}
		maxHeight=max(maxHeight,max(hgt,ofGetHeight()/2.));
		maxWid=max(maxWid,bars[i].updateSize());
	}
	sideBarSpace=maxHeight;
	h=sideBarSpace+bars[0].h*(bars.size()-1);
	w=max(w,maxWid);
	for (unsigned int i=0; i<bars.size(); i++) {
		bars[i].w=maxWid;
	}
}

/*****************************************************************
 * update() :: member of sbGroup
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
 *    NONE :
 *
 */

void sbGroup::update()
{
	for (unsigned int i=0; i<bars.size(); i++) {
		for (unsigned int j=0; j<bars[i].size(); j++) {
			if(bars[i][j].newUpdateHeight())
				updateHeight();
		}
	}
}

/*****************************************************************
 * draw(int _x, int _y) :: member of sbGroup
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    int _x :
 *     int _y :
 *
 *  Output________
 *
 *    NONE :
 *
 */

void sbGroup::draw(int _x, int _y)
{
  area.x=x=_x;
  area.y=y=_y;
  
  area.height=ofGetHeight()-area.y;
  area.width=w;
  
  y+=30;
  
  ofSetColor(gray*1.1);
  ofRect(area);
  
  ofSetColor(black.opacity(.2));
  drawHatching(area.x, area.y, area.width, area.height, 50, 50);
  
  ofSetColor(yellow);
  ofRect(area.x+area.width, area.y, 2, area.height);
  //ofRect(x,y+h,w,1);
  
  ofSetColor(gray);
  ofRect(x,y+h+1,w,15);
  ofSetColor(yellow);
  ofRect(x,y+h+1,w,1);
  ofRect(x,y+h+16,w,1);
  
	draw();
}

/*****************************************************************
 * draw() :: member of sbGroup
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
 *    NONE :
 *
 */


void sbGroup::draw(){
	
	//--------- Draw a gray box onto the sidebar to hold the blocks
	//ofSetColor(0x80633B);
  int binWidth=w;
  for (unsigned int i=0; i<bars.size()-1; i++) {
    if(bars[i].bOpen) ofSetColor((bars[i].color*.5).opacity(.25));
  }
  
	//ofSetColor((white*.2).opacity(.7));
	ofRect(x,y,binWidth,h);
	
	int pos=0;
	for (unsigned int i=0; i<bars.size()-1; i++) {
		bars[i].draw(x,pos+y);
    if(bars[i].bOpen){
      
    }
		pos+=bars[i].h;
		if(bars[i].bOpen) pos+=sideBarSpace;
	}
	bars.back().y=y+h;
}

/*****************************************************************
 * clickDown(double _x, double _y) :: member of sbGroup
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    double _x :
 *     double _y :
 *
 *  Output________
 *
 *    bool :
 *
 */

bool sbGroup::clickDown(double _x, double _y){
	bool ret=false;
	for (unsigned int i=0; i<bars.size()-1; i++) {
		if(bars[i].over(_x,_y)){ 
			for (unsigned int j=0; j<bars.size()-1; j++) {
				if (j!=i) {
					bars[j].bOpen=false;
				}
			}
			ret=bars[i].bOpen=true;
		}
		for (unsigned int j=0; j<bars[i].blocks.size(); j++) {
			if(dest&&bars[i].bOpen) dest->addFromSB(bars[i].blocks[j],_x,_y);
		}
	}
	return ret;
}
