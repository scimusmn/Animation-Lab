/*
 *  controlBar.h
 *  arenaBot
 *
 *  Created by Exhibits on 11/7/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofExtended.h"
#include "../demoAnim/demoAnim.h"
#include "../blockLoad.h"

#include "../serialChecker/serialCheck.h"
#include "../uploadModule/uploadModule.h"
#include "../screenTest/screenTest.h"
#include "../../../dallasEng/dallasEng.h"
#include "../RFID_reader/RFID_reader.h"
#include "../compiler/compiler.h"

class buttonHolder : public ofInterObj {
protected:
  double space;
  double pad;
public:
  buttonHolder();
  buttonHolder(int numberOfObjs, ...);
  ofRectangle area;
  vector<ofInterObj *> objs;
  void addObj(ofInterObj & obj);
  void internalSpace(int space);
  double maxHeight();
  void padding(int pad);
  void draw(int x, int y); 
};

class barOfButtons : public buttonHolder {
  ~barOfButtons(){
    for (unsigned int i=0; i<objs.size(); i++) {
      delete objs[i];
    }
  }
  buttonHolder & operator[](int i);
  void addSection();
  void addSection(int numObjs, ...);
  buttonHolder & lastSection();
};

class controlBar {
protected:
  bGroup * blocks;
  sbGroup * sideBar;
  
  demoAnim anim;
  
  //robotTest testbed;
  
  dallasButton clearBut;
	ofButton redoBut;
	ofButton undoBut;
  dallasButton  demo;
  
  vector<buttonHolder> bHldr;
  
  ofRectangle buttonBar;
  ofRectangle subBar;
  
  ofFont subtitle;
  
  uploadModule upload;
  
  ofButton skipBut;
  
  dallasButton create;
  dallasButton edit;
  
  bool bPluginChoice;
  bool bChooseLevel;
  
  serialCheck serChk;
  
  rootGroup sets;
  
  ofTimer changed;
  ofTimer timeOut;

  compiler cmplr;
public:
  double x,y,w,h;
  //controlBar();
  void loadBlocks(blockGroup & bg);
  void setup(bGroup * bG, sbGroup * sbG);
  void barSpacing();
  void draw(int x, int y);
  void drawForeground();
  void update();
  void setAvailableButtons();
  bool clickDown(int x, int y, int button=0);
  bool clickUp();
  bool mouseLockout(int button);
};
