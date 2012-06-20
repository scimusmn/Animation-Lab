/*
 *  blocks.h
 *  robotBlocks_redux
 *
 *  Created by Exhibits on 1/31/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

/*
 *  RobotGUI.h
 *  
 *
 *  Created by Exhibits on 9/13/10.
 *  Copyright 2010 Science Museum of Minnesota. All rights reserved.
 *
 */

#ifndef _blocks_
#define _blocks_
#include "ofMain.h"
#include "ofExtended.h"
#include <fstream>
#include <algorithm>
#include <deque>

#include "../../dallasEng/dallasEng.h"

//************ type for different kinds of blocks *********
enum blkVectType {
	OF_BLOCK_NULL,OF_BLOCK_ON,OF_BLOCK_IN,OF_BLOCK_NUM
};

enum blkType {
  BLK_DEFAULT,BLK_BRACKET,BLK_SEQ,BLK_VAL,BLK_BASE
};

class bGroup;

/*****************************************************************
 * class block: public ofInterObj 
 *
 *  Description:: 
 *
 *
 *  Vars:
 *
 *   ofColor color::_ _ _ _ _ _ _ _ 
 *   bool grabbed:: _ _ _ _ _ _ _ _ 
 *   bool grabbedOn:: _ _ _ _ _ _ _ 
 *   bool placeHolder:: _ _ _ _ _ _ 
 *   int ddOpen:: _ _ _ _ _ _ _ _ _
 *   bool ddSelected::_ _ _ _ _ _ _ 
 *   bool deleteMe::_ _ _ _ _ _ _ _ 
 *   bool bSeq::_ _ _ _ _ _ _ _ _ _ 
 *   vector<block> numBlocks::_ _ _ 
 *   vector<block> blocksIn:: _ _ _ 
 *   vector<block> blocksOn:: _ _ _ 
 *   ofFontContainer arialHeader::_ 
 *   vector<string> sibling::_ _ _ _ _ 
 *   unsigned char typ::_ _ _ _ _ _ 
 *   vector<ofDropDown> ddGroup:: _ 
 *   int oH:: _ _ _ _ _ _ _ _ _ _ _ 
 *   int titlePos.x::_ _ _ _ _ _ _ _ 
 *   int xo:: _ _ _ _ _ _ _ _ _ _ _ 
 *   string filename::_ _ _ _ _ _ _ 
 *   string title:: _ _ _ _ _ _ _ _ 
 *   placeHolder=true:: _ _ _ _ _ _ 
 */
struct textWord {
	string txt;
	int wid;
	textWord(string t, ofFont& f){
		txt=t;
		wid=f.stringWidth(txt);
	}
};

class block: public ofInterObj {
public:
	ofColor color;
	bool placeHolder;
	int ddOpen;
	bool  ddSelected;
  blkType type;
	vector<block> numBlocks;
	vector<block> blocksIn;
	vector<block> blocksOn;
	ofFont arialHeader;
	vector<string> sibling;
	vector<dallasDrop> ddGroup;

	vector<textWord> ttlSpl;
	int spaceSize;
  
  bGroup * group;
  
  ofPoint titlePos;
  ofPoint ttlSize;
  ofRectangle orig;
  ofRectangle interior;
  double bottomBar;
  
	string filename;
	string title;
  
  ofTag origTag;
  
  string label;
	
	block(ofTag & xml,ofColor col);

	void setup(ofTag & xml,ofColor col);
  
  void parseTitle();
	
	block():ofInterObj(){
		//placeHolder=true;
	}
	
	block(const block &t);
	
	~block();
	
	virtual void setup(double _w, double _h);
	
	void operator=(const block &t);
  
  //*********** draw functions
	
	void drawShadow();
	
	void drawSelected();
	
	void drawDD();
	
	void drawOpenDD();
	
	virtual void draw(bool fade=false);
	
	virtual void draw(int _x, int _y);
	
	int size(int j=0);
	
	int numInside();
  
  int totalInside();
	
	block * topBlock();
	
	block & lastBlock();
	
	block & operator[](int i);
  
  double fullWidth();
	
	int onBlockOn(int x,int y);
  
  bool inOrOn(int x, int y);
	
	bool ddPassingClick(int x, int y);
	
	void motion(int x, int y);
	
	bool siblingWritten(map<string,bool> * printed);
	
	void printOut(ofstream* k,ifstream * f,int t=0, map<string,bool> * printed=0);
	
	void printData(string sibling,ofstream* k,int t=0,map<string,bool> * printed=0, bool printIn=true);
  
  ofTag saveTag();
  
  //--------------------------- New functions --------------------------
  
  bool bGrabbed, null;
  
  int insertSpace;
  
  int newHeightOn();
  
  int newHeightIn();
  
  int newUpdateHeight();
  
  void newUpdatePositions();
  
  virtual bool beneath(block & blockToCheck,signed int spaceBelow=-1);
  
  bool inside(block & t);
  
  bool inBounds(int xX, int yX, int wX, int hX);
  
  bool newClickInside(int _x, int _y);
  
  bool newClickDD(int _x, int _y, bool & ddopen);
  
  virtual bool clickDown(int x, int y);
  
  virtual bool newClickUp(int _x, int _y);
  
  void addBlock(blkVectType bType, int index);
  
  block separateBlock(blkVectType bType, int index);
  
  void removeBlocksByLabel(string label);
  
};

void removeFromVector(vector<block> &,string);
                 
struct dropBlock {
  int index;
  bool bFound;
  block * belowThis;
  vector<block> * inThis;
  blkVectType whichVector;
  dropBlock(){ inThis=0,index=bFound=belowThis=0,whichVector=OF_BLOCK_NULL; }
  dropBlock(block & thisBlock, blkVectType type, int atThisPos){ set(thisBlock,type,atThisPos); }
  void set(block & thisBlock, blkVectType type, int atThisPos){
    bFound=true;
    index=atThisPos;
    belowThis=&thisBlock;
    whichVector=type;
    if(type==OF_BLOCK_ON) inThis=&(thisBlock.blocksOn);
    else if(type==OF_BLOCK_IN) inThis=&(thisBlock.blocksIn);
  }
  bool found() { return bFound; }
};

dropBlock underWhich(block & startBlock, block & dropped);


#endif