/*
 *  sideBar.h
 *  arenaBot
 *
 *  Created by Exhibits on 11/1/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */
#pragma once

#include "../blockGroup/blockGroup.h"

#define as_dynamic(x) static_cast<dynamicSB *>(x)

/*****************************************************************
 * class sideBar: public ofInterObj 
 *
 *  Description:: 
 *
 *
 *  Vars:
 *
 *    public: ofFont arialLabel::_ _ 
 *    ofFont arialHeader:: _ _ _ _ _ 
 *    bool bOpen:: _ _ _ _ _ _ _ _ _ 
 *    float r,g,b::_ _ _ _ _ _ _ _ _ 
 *    ofColor color::_ _ _ _ _ _ _ _ 
 *    int xo,yo::_ _ _ _ _ _ _ _ _ _ 
 *    //bool grabbed:: _ _ _ _ _ _ _ 
 *    string filename::_ _ _ _ _ _ _ 
 *    vector<block> blocks:: _ _ _ _ 
 *    double space:: _ _ _ _ _ _ _ _ 
 *    //int textNum::_ _ _ _ _ _ _ _ 
 *    return blocks[i]:: _ _ _ _ _ _ 
 */

enum barType {
  DEFAULT_BAR, DYNAMIC_BAR
};

class vSideBar : public ofInterObj {
public:
  barType type;
	ofFont arialLabel;
	ofFont arialHeader;
	bool bOpen;
	float r,g,b;
	ofColor color;
	int xo,yo;
	//bool grabbed;
	string filename;
	vector<block> blocks;
	double space;
  vSideBar():ofInterObj(){
  }
  virtual int size()=0;
	
	virtual block & operator[](int i)=0;
	
	virtual double updateSize()=0;	
	virtual void draw(int _x, int _y)=0;
};

class sideBar: public vSideBar {
public:
  
	//int textNum;
	sideBar();
	sideBar(int x,int y,int w,int h,string file,ofColor col);
	sideBar(string title,ofColor col);
  sideBar(ofTag & tag);
	~sideBar();
	void update(double spc);
	
	int size(){ return blocks.size();}
	
	block & operator[](int i){ return blocks[i];}
	
	double updateSize();
	
	void operator=(const sideBar t);
	
	void draw(int _x, int _y);
	
	//void draw(double k,double space);
};

struct deviceBlocks {
  vector<block> blocks;
  double w,h;
  int size(){ return blocks.size();}
  block & operator[](int i){ return blocks[i];}
  deviceBlocks(ofTag & tag,ofColor color,string baseLabel);
};

class dynamicSB : public vSideBar {
protected:
  vector<deviceBlocks> devices;
  dallasDrop select;
  string dropLabel;
public:
  dynamicSB(ofTag & tag);
  int size();
  block & operator[](int i);
  double updateSize();
  void draw(int _x, int _y);
  void operator=(const dynamicSB t);
  bool clickDown(int x, int y);
  vector<block> & set();
};

/*****************************************************************
 * class sbGroup : public ofInterObj 
 *
 *  Description:: 
 *
 *
 *  Vars:
 *
 *    bGroup * dest::_ _ _ _ _ _ _ _
 *    int sideBarSpace:: _ _ _ _ _ _ 
 *    public: vector<sideBar> bars:: 
 *    return bars[i]:: _ _ _ _ _ _ _ 
 *    friend class bGroup::_ _ _ _ _ 
 */


class sbGroup : public ofInterObj {
	bGroup * dest;
	int sideBarSpace;
  double barHeight;
public:
  ofRectangle area;
	vector<vSideBar *> bars;
	void updateBlocks(int i);
	
  sbGroup(){}
	sbGroup(ofXML & xml,bGroup * destin=0);
	
	~sbGroup(){
		bars.clear();
	}
  
  void setup(ofXML & xml,bGroup * destin=0);
  
  void clear();
	
	int size(){ return bars.size();}
	
	vSideBar * operator[](int i){ return bars[i];}
	
	void setDest(bGroup * destin);
	
	void unfold();
	
	void update();
	
	void updateHeight();
	
	void draw();
	
	void draw(int _x, int _y);
  
  void draw(int _x, int _y, int _w, int _h);
	
	bool clickDown(double x, double y);
	
	//int size();
	
	friend class bGroup;
};
