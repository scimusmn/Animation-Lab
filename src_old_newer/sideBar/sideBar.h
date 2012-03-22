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
 *    bool bOpen::_ _ _ _ _ _ _ _ _ _ 
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

class sideBar: public ofInterObj {
public:
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
	//int textNum;
	sideBar();
	sideBar(int x,int y,int w,int h,string file,ofColor col);
	sideBar(string title,ofColor col);
	~sideBar();
	void update(double spc);
	
	int size(){ return blocks.size();}
	
	block & operator[](int i){ return blocks[i];}
	
	double updateSize();
	
	void operator=(const sideBar t);
	
	void draw();
	
	void draw(int _x, int _y);
	
	//void draw(double k,double space);
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
	vector<sideBar> bars;
	void updateBlocks(int i);
	
  sbGroup(){}
	sbGroup(ofXML & xml,bGroup * destin=0);
	
	~sbGroup(){
		bars.clear();
	}
  
  void setup(ofXML & xml,bGroup * destin=0);
  
  void clear();
	
	int size(){ return bars.size();}
	
	sideBar & operator[](int i){ return bars[i];}
	
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
