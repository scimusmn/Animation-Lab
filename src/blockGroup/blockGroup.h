/*
 *  blockGroup.h
 *  arenaBot
 *
 *  Created by Exhibits on 11/1/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#pragma once
#include "../blocks.h"
#include "../baseBlock/baseBlock.h"
#include "../bracketBlock/bracketBlock.h"

/*****************************************************************
 * struct storageState 
 *
 *  Description:: 
 *
 *
 *  Vars:
 *
 *    vector<block> blocks:: _ _ _ _ 
 *    block base:: _ _ _ _ _ _ _ _ _ 
 */

struct storageState {
  vector<block> blocks;
  baseBlock base;
  storageState(){}
  storageState(vector<block> & k, baseBlock b){
    blocks=k;
    base=b;
  }
};

/*****************************************************************
 * class bGroup: public ofInterObj 
 *
 *  Description:: 
 *
 *
 *  Vars:
 *
 *    vector<block> blocks:: _ _ _ _ _ _ 
 *    map<string,bool> used::_ _ _ _ _ _
 *    deque<storageState> storedState::_
 *    int stateCount:: _ _ _ _ _ _ _ _ _
 *    ofImage rTop:: _ _ _ _ _ _ _ _ _ _
 *    ofImage rBot:: _ _ _ _ _ _ _ _ _ _
 *    ofImage rSide::_ _ _ _ _ _ _ _ _ _
 *    public: block base:: _ _ _ _ _ _ _
 *    clock_t dblClick:: _ _ _ _ _ _ _ _
 *    int lastBlock::_ _ _ _ _ _ _ _ _ _
 *    bool grabbed,inHand,ddopen:: _ _ _
 *    double dispx, dispy::_ _ _ _ _ _ _
 *    double placex, placey::_ _ _ _ _ _
 */

class bGroup: public ofInterObj {
	map<string,bool> used;
	//deque<storageState> storedState;
	//int stateCount;
  stateSave<storageState> states;
	ofImage rTop;
	ofImage	rBot;
	ofImage rSide;
  
  block * current;
  
  ofTimer actionTime;
  bool bSequencePlay;
  bool bTesting;
  bool bChanged;
  dallasScroll bar;
  
  ofFont label;
  
  bool bScrolling;
public:
  
  vector<block> blocks;
	baseBlock base;
	clock_t dblClick;
	int lastBlock;
	bool bGrabbed,inHand,ddopen;
	double dispx, dispy;
	double placex, placey;
	bGroup(double x, double y,double wid,double hgt);
	
	bGroup();
	
	~bGroup();
  
  void setScrolling(bool scrl);
	
	void recordState();
	
	void undoState();
	
	void redoState();
	
	bool undoAvailable();
	
	bool redoAvailable();
	
	void setup(double x, double y,double wid,double hgt);
	
	int size();
	
	block operator[](int i);
  
  int totalBlocks();
  
  
  void saveXML(string filename);
  
  void loadFile(string filename);
  
  bool changedSinceSave();
  
	
	void addFromSB(block t,int x,int y);
	
	void writeFile(ofstream *k);
  
  void writeFile(string filname);
	
	void resetUsed(block & t);
	
	void resetUsed();
	
	void draw();
  
  void draw(int _x, int _y, int _w, int _h);
	
	void drawBase(int _x, int _y);
	
	bool drawIndicators(block & grab,block & k);
	
	void update();
	
	void addNum(block & holder, block & held);
	
	void clear();
	
	void drawForeground();
	
	void drag(double x, double y);
	
	void motion(double x, double y);
	
	int handleClickUps(block & first, block & second, bool checkOn=true);
	
	int above(int i, int j);
  
  //--------------------- New functions ------------------------
  
  block held;
  
  bool clickDown(int _x, int _y);
  
  bool newClickUp(int _x, int _y);
  
  bool newHandleClickUp(block & grab, block & chk);
  
  bool newHandleClick(vector<block> & chk, int indx, int _x, int _y, bool top=false);
  
  bool processBlockDrop(block & drop,block & target, bool reverse=false);
  
  bool pushBlocks(block & dropped, vector<block> & into, int i, bool top=false);
  
  void pullBlocks(vector<block> & chk, int i);
  
  void resize(int x, int y, int w, int h);
  
  void setFocus(block * cur);
  
  void loseFocus();
  
};

//bGroup & blocks();