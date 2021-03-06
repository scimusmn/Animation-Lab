/*
 *  sideBar.cpp
 *  robotBlocksVectors
 *
 *  Created by Exhibits on 3/10/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "sideBar.h"
#include "../robotConfig.h"

extern string defaultFont;

extern ofColor white, black, gray, yellow, blue, red, orange;

void vSideBar::drawButton(int _x, int _y, int _w, int _h, int fontSize)
{
	button.cSetup(_x,_y,_w,_h);
	ofFontMode oldH=arialHeader.getHorMode();
	ofFontMode oldV=arialHeader.getVertMode();
	arialHeader.setSize(fontSize);
	arialHeader.setMode(OF_FONT_CENTER);
	arialHeader.setMode(OF_FONT_TOP);
	ofSetColor(color-(!bOpen?0:.2*255));
	ofRoundedRect(_x,_y,_w,_h,_h/2);
	if(bOpen) ofSetColor(blue),ofSetLineWidth(8);
	else ofSetColor(gray);
	ofNoFill();
	ofRoundedRect(_x,_y,_w,_h,_h/2);
	ofSetLineWidth(1);
	ofFill();
	ofSetColor(white);
	arialHeader.drawString(filename,_x+_w/2,_y+(_h-arialHeader.stringHeight(filename))/2);
	arialHeader.setMode(oldH);
	arialHeader.setMode(oldV);
	arialHeader.setSize(cfg().buttonFontSize);
}

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

sideBar::sideBar(int _x,int _y,int w,int h,string file,ofColor col):vSideBar(){
	arialHeader.loadFont("fonts/HelveticaBold.otf");
	arialHeader.setSize(cfg().buttonFontSize);
	bOver=bPressed=false;
	bOpen = false;
	xo=_x;
	yo=_y;
	color=col;
	filename=file;
}

sideBar::sideBar(string title,ofColor col):vSideBar(){
	arialHeader.loadFont("fonts/HelveticaBold.otf");
	arialHeader.setSize(cfg().buttonFontSize);
	bOver=bPressed=false;
	bOpen = false;
	color=col.opacity(255);
  h=arialHeader.stringHeight("Kjhg")*1.3;
	filename=title;
}

sideBar::sideBar(ofTag & tag):vSideBar()
{
  type=DEFAULT_BAR;
  arialHeader.loadFont("fonts/HelveticaBold.otf");
	arialHeader.setSize(cfg().buttonFontSize);
	bOver=bPressed=false;
	bOpen = false;
  h=arialHeader.stringHeight("Kjhg")*1.3;
  color.setHex(strtol(tag.getAttribute("color").c_str(),NULL,0));
  filename=tag.getAttribute("name");
  w=0;
  for (unsigned int j=0; j<tag.size(); j++) {
    if (tag[j].getLabel()=="block") {
        blocks.push_back(block(tag[j],color));
        w=max(w,blocks.back().fullWidth());
    }
  }
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

sideBar::sideBar():vSideBar(){
	arialHeader.loadFont(defaultFont);
	arialHeader.setSize(cfg().buttonFontSize);
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
  type=t.type;
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
	if(!cfg().buttonsOnSidebar){
		ofSetColor(gray);
		trimmedRect(x, y, w, h);
		ofSetColor(color);
		if(!bOpen) trimmedRect(x,y,w/8,h);
		else trimmedRect(x,y,w,h);
		ofSetColor(cfg().lineColor);
		ofNoFill();
		trimmedRect(x,y,w,h);
		ofFill();
		ofSetColor(white);
		arialHeader.drawString(filename,x+w/8+10,y+3*h/4);
	}
	int pad=20;
	if(bOpen){
		int temp=y+(!cfg().buttonsOnSidebar?h:0)+pad;
		for (unsigned int j=0; j<blocks.size(); j++) {
			blocks[j].draw(x+pad,temp);
			if(j<blocks.size()-1){
				ofSetColor(black.opacity(128));
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


/****************************** Dynamic Sidebar **********************
 *********************************************************************/

deviceBlocks::deviceBlocks(ofTag & tag, ofColor color,string baseLabel)
{
  w=h=0;
  string secondLabel=tag.getAttribute("label");
  string src=tag.getAttribute("source");
  if(src.length()){
    ofXML k;
    k.loadFile(cfg().robotRoot+"/xmlSources/"+src);
    k.setCurrentTag(";");
    tag=k.getCurrentTag();
  }
  for (unsigned int j=0; j<tag.size(); j++) {
    if (tag[j].getLabel()=="block") {
      blocks.push_back(block(tag[j],color));
	  blocks.back().label=baseLabel+":"+secondLabel+":"+tag[j].getAttribute("label");
      blocks.back().blocksOn.clear();
      w=max(w,blocks.back().fullWidth());
    }
  }
}

dynamicSB::dynamicSB(ofTag & tag):vSideBar()
{
  string oldLabel=tag.getAttribute("label");
  filename=tag.getAttribute("name");
  string colorString=tag.getAttribute("color");
  color.setHex(strtol(colorString.c_str(),NULL,0));
  
  string src=tag.getAttribute("source");
  if(src.length()){
    ofXML k;
    k.loadFile(cfg().robotRoot+"/xmlSources/"+src);
    k.setCurrentTag(";");
    tag=k.getCurrentTag();
    tag.addAttribute("label", oldLabel);
    tag.addAttribute("name", filename);
    tag.addAttribute("color", colorString);
  }
  
  pad=10;
  type=DYNAMIC_BAR;
  arialHeader.loadFont("fonts/HelveticaBold.otf");
  arialHeader.setSize(cfg().buttonFontSize);
  arialHeader.setMode(OF_FONT_MID);
	bOver=bPressed=false;
	bOpen = false;
  h=arialHeader.stringHeight("Kjhg")*1.3;
  w=0;
  select.dallasStyle();
  select.arial.setSize(cfg().buttonFontSize-4);
  select.setMode(false);
  for (unsigned int j=0; j<tag.size(); j++) {
    if(tag[j].getLabel()=="dropdown"){
      select.init(tag[j]);
    }
    if (tag[j].getLabel()=="device") {
      devices.push_back(deviceBlocks(tag[j],color,tag.getAttribute("label")));
      w=max(w,devices.back().w+pad*4);
    }
  }
  arialHeader.setSize(cfg().buttonFontSize-4);
  arialHeader.setMode(OF_FONT_MID);
  dropOffset=arialHeader.stringWidth(filename+" is a(n)");
  arialHeader.setSize(cfg().buttonFontSize);
  
  w=max(w,dropOffset+select.w+pad*3);
}


int dynamicSB::size()
{
  return set().size();
}

block & dynamicSB::operator[](int i)
{
  return set()[i];
}

double dynamicSB::updateSize()
{
  for (unsigned int i=0; i<set().size(); i++) {
		if (set()[i].w>3*w/4) {
			w=max(w,set()[i].w+40);
		}
	}
	return w;
}

void dynamicSB::draw(int _x, int _y)
{
  x=_x, y=_y;
  if(bOpen){
    if(cfg().defaultColor) ofSetColor(gray);
	else ofSetColor(cfg().subtitleColor);
    ofRectangle k(x, y+(!cfg().buttonsOnSidebar?h:0), w, select.h+pad*2);
    ofRect(k.x,k.y,k.width,k.height);
    ofSetColor(black);
    if(cfg().defaultColor) drawHatching(k.x,k.y,k.width,k.height, 1, 15);
    drawBorder(k);
    if(cfg().defaultColor) ofSetColor(yellow);
	else ofSetColor(cfg().textColor);
    arialHeader.setSize(cfg().buttonFontSize-4);
    arialHeader.setMode(OF_FONT_MID);
    arialHeader.drawString(filename+" is a(n)",x+pad,k.y+(select.h+pad*2)/2);
    arialHeader.setSize(cfg().buttonFontSize);
	select.draw(x+dropOffset+pad*2, k.y+pad);
  }
	
	if(!cfg().buttonsOnSidebar){
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
		arialHeader.drawString(filename+" - Choose device",x+w/8+10,y+h/2);
	}
	if(bOpen){
		int temp=(!cfg().buttonsOnSidebar?h:0);
		temp+=y+select.h+pad*3;
		for (unsigned int j=0; j<set().size(); j++) {
			set()[j].draw(x+pad*2,temp);
			if(j<set().size()-1){
				ofSetColor(black.opacity(128));
				ofRect(x, temp+set()[j].h+set()[j].newHeightOn()+pad, w, 1);
			}
			temp+=set()[j].h+set()[j].newHeightOn()+pad*2;
		}
		select.draw(x+dropOffset+pad*2, y+(!cfg().buttonsOnSidebar?h:0)+pad);
	}
}

void dynamicSB::operator=(const dynamicSB t)
{
  x=t.x;
	y=t.y;
	w=t.w;
	h=t.h;
  color=t.color;
  filename=t.filename;
  devices=t.devices;
  select=t.select;
  type=t.type;
}

bool dynamicSB::clickDown(int _x, int _y)
{
  bool ret=0;
  if(bOpen) select.clickDown(_x, _y);
  if(bOpen&&select.justSelected()){
    ret=1;
  }
  return ret;
}

vector<block> & dynamicSB::set()
{
  return devices[select.getChoiceNumber()].blocks;
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
	int divs=bars[i]->blocks.size();
	for (unsigned int j=0; j<divs; j++) {
		int w=bars[i]->blocks[j].w;
		bars[i]->blocks[j].x=(bars[i]->w-30-w)/2;
		bars[i]->blocks[j].y=bars[i]->y+5*bars[i]->h/4+sideBarSpace*j*\
		((bars[i+1]->y-(bars[i]->y+bars[i]->h))/sideBarSpace)/divs;
		bars[i]->blocks[j].h=bars[i]->blocks[j].orig.height*((bars[i+1]->y-(bars[i]->y+bars[i]->h))/sideBarSpace);
	}
}


void sbGroup::setup(ofXML & xml,bGroup * destin)
{
  dest=destin;
  xml.setCurrentTag(";blocks");
	string font=xml.getCurrentTag().getAttribute("font");
	ofTag & tag=xml.getCurrentTag();
  int numDyn=0;
	for (unsigned int i=0; i<tag.size(); i++) {
		if (tag[i].getLabel()=="bar") {
			if(tag[i].getAttribute("type")=="default"||tag[i].getAttribute("type")=="") bars.push_back(new sideBar(tag[i]));
			else if(tag[i].getAttribute("type")=="dynamic"){
				bars.push_back(new dynamicSB(tag[i]));
				as_dynamic(bars.back())->select.setSelected(numDyn);
				numDyn++;
			}

			w=max(bars.back()->w,w);
		}
	}
	bars.push_back(new sideBar("Filler",ofColor(0,0,0)));
	if (bars.size()) {
		y=bars[0]->y;
		x=0;
    barHeight=bars[0]->h;
	}
  for (unsigned int i=0; i<bars.size(); i++) {
    bars[i]->w=w;
  }
	updateHeight();
	if(bars.size()>=2) bars[0]->bOpen=true;
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
 *    bGroup * destin :
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
		for (unsigned int j=0; j<bars[i]->size(); j++) {
			hgt+=(*bars[i])[j].h+(*bars[i])[j].newHeightOn()+20;
		}
		maxHeight=max(maxHeight,max(hgt,ofGetHeight()/3.));
		maxWid=max(maxWid,bars[i]->updateSize());
	}
	sideBarSpace=maxHeight;
	if(bars.size()) h=sideBarSpace+bars[0]->h*(bars.size()-1);
	w=max(w,maxWid);
	for (unsigned int i=0; i<bars.size(); i++) {
		bars[i]->w=maxWid;
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
		for (unsigned int j=0; j<bars[i]->size(); j++) {
			if((*bars[i])[j].newUpdateHeight())
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
  
  int pad=10;
  int buttonH=30;

  if(cfg().buttonsOnSidebar) y+=pad*(bars.size()/2+2)+(bars.size()/2)*buttonH;
  else y+=30;
  if(cfg().defaultColor)  ofSetColor(gray);
  else ofSetColor(cfg().sideBarColor);
  ofRect(area.x,area.y,area.width,area.height);
  
  ofSetColor(black.opacity(50));
  if(cfg().defaultColor) drawHatching(area.x, area.y, area.width, area.height, 50, 50);

  if(cfg().buttonsOnSidebar){
	  //ofSetColor(black.opacity(64));
	  //ofRect(x,area.y,w,y-area.y);
	  if(!cfg().defaultColor){
		//ofSetColor(cfg().lineColor.opacity(.1));
		//drawHatching(x,area.y,w,y-area.y, 1,1);
	}
  }
  
  if(cfg().defaultColor) ofSetColor(yellow);
  else ofSetColor(cfg().lineColor);
  ofRect(area.x+area.width, area.y, 2, area.height);
  ofRect(x,y,w,2);
  //ofRect(x,y+h,w,1);
  
  if(cfg().defaultColor) ofSetColor(gray);
  else ofSetColor(cfg().subtitleColor);
  ofRect(x,y+h+1,w,15);
  if(cfg().defaultColor) ofSetColor(yellow);
  else ofSetColor(cfg().lineColor);
  ofRect(x,y+h+1,w,1);
  ofRect(x,y+h+16,w,1);
  
  if(cfg().buttonsOnSidebar)
		for (unsigned int i=0; i<bars.size()-1; i++) {
			if(i<bars.size()-2||!((bars.size()-1)%2)) bars[i]->drawButton(x+(pad*(i%2+1))+(i%2)*((w-3*pad)/2),area.y+pad*(i/2+1)+(i/2)*buttonH,((w-3*pad)/2),buttonH,cfg().buttonFontSize-3);
			else bars[i]->drawButton(x+(w-((w-3*pad)/2))/2,area.y+pad*(i/2+1)+(i/2)*buttonH,((w-3*pad)/2),buttonH,cfg().buttonFontSize-3);
		}

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
		if(bars[i]->bOpen) ofSetColor((bars[i]->color*.75).opacity(128));
	}
  
	//ofSetColor((white*.2).opacity(.7));
	ofRect(x,y,binWidth,h);
	
	if(!cfg().defaultColor){
		ofSetColor(cfg().lineColor.opacity(25));
		drawHatching(x,y,binWidth,h, 7,1);
	}
	
	int pos=0;
	for (unsigned int i=0; i<bars.size()-1; i++) {
		bars[i]->draw(x,y+(cfg().buttonsOnSidebar?0:pos));

		if(bars[i]->bOpen){
			
		}
		pos+=bars[i]->h;
		if(bars[i]->bOpen) pos+=sideBarSpace;
	}
	bars.back()->y=y+h;
}

vSideBar * sbGroup::openBar()
{
	vSideBar * ret=0;
	for (unsigned int i=0; i<bars.size()-1; i++) {
		if(bars[i]->bOpen) ret=bars[i];
	}
	return ret;
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
	bool & BOSB=cfg().buttonsOnSidebar;
	for (unsigned int i=0; i<bars.size()-1; i++) {
		if((!BOSB&&bars[i]->over(_x,_y))||(BOSB&&bars[i]->button.over(_x,_y))){ 
			for (unsigned int j=0; j<bars.size()-1; j++) {
				if (j!=i) {
					bars[j]->bOpen=false;
				}
			}
			ret=bars[i]->bOpen=true;
		}
    if(bars[i]->type==DYNAMIC_BAR){
      dynamicSB & dsb= *as_dynamic(bars[i]);
      string lbl=dsb.set()[0].label;
      if(dsb.clickDown(_x,_y)){
        if(dsb.set()[0].label!=lbl){
          dest->clearBlocksByLabel(lbl);
        }
        ret=true;
      }
    }
		for (unsigned int j=0; j<bars[i]->size(); j++) {
			if(dest&&bars[i]->bOpen&&!ret){
        if((*bars[i])[j].over(_x,_y)||(*bars[i])[j].onBlockOn(_x, _y)){
          ret=true;
          dest->addFromSB((*bars[i])[j],_x,_y);
        }
      }
		}
	}
	return ret;
}
