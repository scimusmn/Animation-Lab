/*
 *  blocks.cpp
 *  robotBlocks_redux
 *
 *  Created by Aaron Heidgerken-Greene on 1/31/2011.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "blocks.h"
#include "blockDraw.h"

/*
 *  RobotGUI.h
 *  
 *
 *  Created by Exhibits on 9/13/10.
 *  Copyright 2010 Science Museum of Minnesota. All rights reserved.
 *
 */

#ifndef _block_
#define _block_
#include <fstream>
#include <vector>
#include <algorithm>
#include "robotConfig.h"

int pixPerInch=25;

int TITLE_HEIGHT=40;

string defaultFont="fonts/HelveticaBold.otf";

/*****************************************************************
 * block(ofTag & cur,ofColor col, int _y):ofInterObj(-200,-200,150,45) :: constructor for block, a subclass of ofInterObj
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *
 *  Output________
 *
 *    New instance of block :
 *
 */

void block::setup(ofTag & cur,ofColor col){
	w=150;
	//********* This is the method by which all of the blocks are first generated from the xml files in the data root.
	//-------- TODO: get rid of the garbage with the color triples. blech.
	//-------- load the font for the arialHeader, at 10 pt.
	
	origTag=cur;
  
	arialHeader.loadFont(defaultFont);
  arialHeader.setMode(OF_FONT_MID);
	arialHeader.setSize(cfg().blockFontSize);
	insertSpace=0;
  bGrabbed=false;
	//-------- color initialization
	if(cur.getAttribute("color").length())
		color=ofColor(strtol(cur.getAttribute("color").c_str(),NULL,0));
	else color=col;
	
	//-------- load name from the name of the xmlNode
	title=cur.getAttribute("name");

	h=TITLE_HEIGHT=arialHeader.stringHeight("Kjhg")*2;

  ttlSize.x=w;
  ttlSize.y=TITLE_HEIGHT;
  ddSelected=false;
  
	//cout << title << endl;
	
	//-------- init some variables, to prevent garbage from happening
	ddOpen=false;
	titlePos.x=10;
  
  type=BLK_DEFAULT;
  
	placeHolder=false;
	//-------- declare the map used for the switch
	map<string,int> list;
	list["seq"]=0;
	list["bracket"]=1;
	list["action"]=4;
	list["file"]=5;
	list["sibling"]=6;
	list["num"]=7;
	list["dropdown"]=8;
	list["blocksIn"]=9;
  list["blocksOn"]=10;
	for(int i=0;i<cur.size();i++){
		string node[2]={cur[i].getLabel(),cur[i].getValue()};
		//-- node[0] is the label, node[1] is the value
    if(list.find(node[0])!=list.end()){
      switch (list.find(node[0])->second) {
        case 1: //-- bracket
          //-------- set type to bracket, and change size
          type=BLK_BRACKET;
          h=2*ttlSize.y+25;
          w=200;
          titlePos.x=30;
          break;
        case 5: // file
          //-- definitely not deprecated, used to store value of which file to write from
          filename=node[1];
          break;
        case 6: // sibling
          //-- stores the name of the complement blocks
          sibling.push_back(node[1]);
          break;
        case 7: // num
          //-- set the statement block flag
          type=BLK_VAL;
          titlePos.x=0;
          ttlSize.x=w=90;
          ttlSize.y=h=20;
          break;
        case 8: // dropdown
          //-- add a new dropdown menu to the block
          ddGroup.push_back(dallasDrop(cur[i]));
		  ddGroup.back().arial.setSize(cfg().blockFontSize-4);
          break;
        case 9:
          for (unsigned int j=0; j<cur[i].size(); j++) {
            if (cur[i][j].getLabel()=="block") {
              blocksIn.push_back(block());
			  blocksIn.back().setup(cur[i][j],color);
            }
          }
          break;
        case 10:
          for (unsigned int j=0; j<cur[i].size(); j++) {
            if (cur[i][j].getLabel()=="block") {
              blocksOn.push_back(block());
			  blocksOn.back().setup(cur[i][j],color);
            }
          }
          break;
        default:
          break;
      }
    }
	}
  
  parseTitle();
}


block::block(ofTag & cur,ofColor col):ofInterObj(-200,-200,150,TITLE_HEIGHT) {
	//********* This is the method by which all of the blocks are first generated from the xml files in the data root.
	//-------- TODO: get rid of the garbage with the color triples. blech.
	//-------- load the font for the arialHeader, at 10 pt.
  
  string src=cur.getAttribute("source");
  
  if(src.length()){
    ofXML xml;
    xml.loadFile(cfg().robotRoot+"/xmlSources/"+src);
    xml.setCurrentTag(";");
    cur=xml.getCurrentTag();
  }
	
	origTag=cur;
  
	arialHeader.loadFont(defaultFont);
  arialHeader.setMode(OF_FONT_MID);
	arialHeader.setSize(cfg().blockFontSize);
	insertSpace=0;
  bGrabbed=false;
	//-------- color initialization
	if(cur.getAttribute("color").length())
		color=ofColor(strtol(cur.getAttribute("color").c_str(),NULL,0));
	else color=col;
	
	//-------- load name from the name of the xmlNode
	title=cur.getAttribute("name");

	h=TITLE_HEIGHT=arialHeader.stringHeight("Kjhg")*2;

  ttlSize.x=w;
  ttlSize.y=TITLE_HEIGHT;
  ddSelected=false;
  
	//cout << title << endl;
	
	//-------- init some variables, to prevent garbage from happening
	ddOpen=false;
	titlePos.x=10;
  
  type=BLK_DEFAULT;
  
	placeHolder=false;
	//-------- declare the map used for the switch
	map<string,int> list;
	list["seq"]=0;
	list["bracket"]=1;
	list["action"]=4;
	list["file"]=5;
	list["sibling"]=6;
	list["num"]=7;
	list["dropdown"]=8;
	list["blocksIn"]=9;
  list["blocksOn"]=10;
	for(int i=0;i<cur.size();i++){
		string node[2]={cur[i].getLabel(),cur[i].getValue()};
		//-- node[0] is the label, node[1] is the value
    if(list.find(node[0])!=list.end()){
      switch (list.find(node[0])->second) {
        case 1: //-- bracket
          //-------- set type to bracket, and change size
          type=BLK_BRACKET;
          h=2*ttlSize.y+25;
          w=200;
          titlePos.x=30;
          break;
        case 5: // file
          //-- definitely not deprecated, used to store value of which file to write from
          filename=node[1];
          break;
        case 6: // sibling
          //-- stores the name of the complement blocks
          sibling.push_back(node[1]);
          break;
        case 7: // num
          //-- set the statement block flag
          type=BLK_VAL;
          titlePos.x=0;
          ttlSize.x=w=90;
          ttlSize.y=h=20;
          break;
        case 8: // dropdown
          //-- add a new dropdown menu to the block
          ddGroup.push_back(dallasDrop(cur[i]));
		  ddGroup.back().arial.setSize(cfg().blockFontSize-4);
          break;
        case 9:
          for (unsigned int j=0; j<cur[i].size(); j++) {
            if (cur[i][j].getLabel()=="block") {
              blocksIn.push_back(block(cur[i][j],color));
            }
          }
          break;
        case 10:
          for (unsigned int j=0; j<cur[i].size(); j++) {
            if (cur[i][j].getLabel()=="block") {
              blocksOn.push_back(block(cur[i][j],color));
            }
          }
          break;
        default:
          break;
      }
    }
	}
  
  parseTitle();
}

void block::parseTitle()
{
	for(int i=0; i<ddGroup.size(); i++)
		ddGroup[i].changeSize(ddGroup[i].w, (ddGroup[i].arial.stringHeight("1")+4));
  
  orig.height=h;
  orig.width=w;
  titlePos.y=(ttlSize.y)/2;
  if(type==BLK_BRACKET){
    interior.x=20;
    interior.y=TITLE_HEIGHT;
    interior.height=TITLE_HEIGHT;
    bottomBar=orig.height-(interior.y+interior.height);
  }
  else interior.x=interior.y=interior.height=bottomBar=0;
	int ddNum=0;
	
	//-------- assign a default value to the xdis of each dd
	for (unsigned int i=0; i<ddGroup.size(); i++) {
		ddGroup[i].relPos.x=0;
		ddGroup[i].relPos.y=(ttlSize.y-ddGroup[i].h)/2;
		ddGroup[i].setMode(false);
	}
	
	//-------- change the font size if it is a statement block
	if(type==BLK_VAL){
		arialHeader.setSize(7);
	}
	
	//-------- split the title into words by looking for " ", and establish a baseline for the width with "s"
	vector<string> titleSplit = ofSplitString(title, " ");
	for(unsigned int i=0; i<titleSplit.size(); i++){
		ttlSpl.push_back(textWord(titleSplit[i],arialHeader));
	}
	int sp=1;
	spaceSize=arialHeader.stringWidth("i");
	int totalwidth=titlePos.x;
	
	//-------- set the displacement for each object in the title, statement blocks and dropdowns
	for (unsigned int i=0; i<ttlSpl.size(); i++) {
		if(!ttlSpl[i].txt.compare("%d")){
			if(ddNum<ddGroup.size()){
				//-------- augment the relative position with the current total width
				ddGroup[ddNum].relPos.x+=totalwidth;
				ttlSpl[i].wid=ddGroup[ddNum].w;
				//-------- update total width
				totalwidth+=ddGroup[ddNum].w+spaceSize;
				//-------- if you have two dropdowns in a row, make sure they don't overlap
				if((i<=ttlSpl.size()-2&&!ttlSpl[i+1].txt.compare("%d"))){
					//ddGroup[ddNum].relPos.x+=spaceSize;
					totalwidth+=spaceSize;
				}
				ddNum++;
			}
		}
		else if(!ttlSpl[i].txt.compare("%b")){
			//-------- if we find a statement block, init it as a placeholder
			int cur=numBlocks.size();
			numBlocks.push_back(block());
			numBlocks[cur].x=totalwidth+titlePos.x;
			numBlocks[cur].relPos.x=numBlocks[cur].x;
			numBlocks[cur].w=50;
			numBlocks[cur].h=20;
			if(h<=5+numBlocks[cur].h) h=5+numBlocks[cur].h;
			totalwidth+=50+spaceSize;
		}
		else {
			totalwidth+=arialHeader.stringWidth(ttlSpl[i].txt);
			if(i<ttlSpl.size()-1) totalwidth+=spaceSize;
		}
	}
	double newWid=titlePos.x;
	for(unsigned int i=0; i<ttlSpl.size(); i++){
		newWid+=ttlSpl[i].wid;
		if(i<ttlSpl.size()-1) newWid+=spaceSize;
	}
	if(type!=BLK_VAL) w=max(newWid+10,w);
	else {
		w=newWid-10;
	}
  
	orig.height=h;
}

/*****************************************************************
 * block(const block &t) :: constructor for block
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    const block &t :
 *
 *  Output________
 *
 *    New instance of block :
 *
 */

block::block(const block &t){
	*this=t;
}

/*****************************************************************
 * ~block() :: deconstructor for block
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
 *    New instance of block :
 *
 */

block::~block() {
	blocksOn.clear();
	blocksIn.clear();
	ddGroup.clear();
}

/*****************************************************************
 * operator=(const block &t) :: function of block
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    const block &t :
 *
 *  Output________
 *
 *    NONE :
 *
 */

void block::operator=(const block &t) {
	x=t.x;
	y=t.y;
	w=t.w;
	h=t.h;
  relPos=t.relPos;
  
  orig=t.orig;
  interior=t.interior;
  titlePos=t.titlePos;
  ttlSize=t.ttlSize;
  bottomBar=t.bottomBar;
  
	arialHeader=t.arialHeader;
	type=t.type;
	bGrabbed=t.bGrabbed;
	title =t.title;
	filename=t.filename;
	numBlocks=t.numBlocks;
	
	sibling=t.sibling;

	ddGroup=t.ddGroup;
	blocksOn=t.blocksOn;
	blocksIn=t.blocksIn;
	ddOpen=t.ddOpen;
  ddSelected=t.ddSelected;
	placeHolder=t.placeHolder;
	color=t.color;
  insertSpace=t.insertSpace;
  ttlSpl=t.ttlSpl;
  spaceSize=t.spaceSize;
  
  origTag=t.origTag;
  label=t.label;
}

/*****************************************************************
 * setup(double _w, double _h) :: function of block
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    double _w :
 *     double _h :
 *
 *  Output________
 *
 *    NONE :
 *
 */

void block::setup(double _w, double _h)
{
	w=_w, h=_h;
  orig.width=w;
  orig.height=h;
  interior.height=bottomBar=0;
  interior.y=0;
  interior.x=0;
	arialHeader.loadFont("DinC.ttf");
	arialHeader.setSize(21);
	//title="to program, connect blocks here";
	//title="connect blocks here and press button";
  title="";
  type=BLK_BASE;
	bGrabbed=0;
	w=max(w,double(arialHeader.stringWidth(title)+20));
	ddOpen=false;
	titlePos.x=10;
	color.set(0xdbb700);
	filename="null";
	placeHolder=false;
  
  ttlSize.y=h/2;
  ttlSize.x=_w;
  label="";
}

//--------------- Maybe unnecessary functions? --------------

int block::size(int j){
	return blocksOn.size();
}

int block::numInside(){
	return blocksIn.size();
}

int block::totalInside()
{
  int ret=0;
  for (unsigned int i=0; i<blocksIn.size(); i++) {
    ret+=1+blocksIn[i].totalInside();
  }
  return ret;
}

block & block::lastBlock(){
	if (blocksOn.size()) return blocksOn.back();
	else return *this;
}

block & block::operator[](int i){
	return blocksOn[i];
}

void removeFromVector(vector<block> & blks,string label){
  for (unsigned int i=0; i<blks.size(); i++) {
    if(blks[i].label==label){
      blks.erase(blks.begin()+i);
      i--;
    }
  }
}

void block::removeBlocksByLabel(string label){
  for (unsigned int i=0; i<blocksOn.size(); i++) {
    blocksOn[i].removeBlocksByLabel(label);
    removeFromVector(blocksOn,label);
  }
  for (unsigned int i=0; i<blocksIn.size(); i++) {
    blocksIn[i].removeBlocksByLabel(label);
    removeFromVector(blocksIn,label);
  }
  
}

#endif