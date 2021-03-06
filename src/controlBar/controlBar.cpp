/*
 *  controlBar.cpp
 *  arenaBot
 *
 *  Created by Exhibits on 11/7/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "controlBar.h"
#include "../sideBar/sideBar.h"
#include "../robotConfig.h"
#include "../screenTest/screenTest.h"
#include "../screenReport/screenReport.h"

extern ofColor black, white, gray,blue,yellow,orange, red;

extern int CURRENT_ROBOT_NUMBER;

string ROOT_TITLE="";
string ROOT_DIR="";

//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_
//_-_-_-_-_//_-_-_-_-_button Hldr//_-_-_-_-_//_-_-_-_-_

buttonHolder::buttonHolder()
{
  area.width=0;
  area.height=0;
}

buttonHolder::buttonHolder(int numObj, ...)
{
  area.height=0;
  va_list vl;
	va_start(vl,numObj);
  
  for (int i=0; i<numObj; i++) {
    addObj(*va_arg(vl,ofInterObj *));
  }
  
  va_end(vl);
}

void buttonHolder::addObj(ofInterObj & obj)
{
  objs.push_back(&obj);
  area.width+=obj.w;
}

void buttonHolder::internalSpace(int spc)
{
  space=spc;
  area.width+=space*(objs.size()-1);
}

double buttonHolder::maxHeight()
{
  double ret=0;
  for (unsigned int i=0; i<objs.size(); i++) {
    ret=max(ret,objs[i]->h);
  }
  return ret;
}

void buttonHolder::padding(int pd)
{
  pad=pd;
  area.width+=pad;
}

void buttonHolder::draw(int _x, int _y)
{
  drawBorder(area);
  area.x=x=_x;
  area.y=y=_y;
  int xPos=pad/2;
  for (unsigned int i=0; i<objs.size(); i++) {
    objs[i]->draw(x+xPos,y+(area.height-objs[i]->h)/2);
    xPos+=objs[i]->w+space;
  }
}

buttonHolder & barOfButtons::operator[](int i){
  return *static_cast<buttonHolder *>(objs[i]);
}

void barOfButtons::addSection()
{
  objs.push_back(new buttonHolder());
}

void barOfButtons::addSection(int numObjs, ...)
{
  va_list vl;
	va_start(vl,numObjs);
  
  objs.push_back(new buttonHolder(numObjs, vl));
  va_end(vl);
}

buttonHolder & barOfButtons::lastSection()
{
  return (*this)[objs.size()-1];
}

//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_
//_-_-_-_-_//_-_-_-_-_control Bar//_-_-_-_-_//_-_-_-_-_


void controlBar::barSpacing()
{
  //_-_-_-_-_//_-_-_-_-_ spacing setup //_-_-_-_-_//_-_-_-_-_
  for(int i=0; i<3;i++)
    bHldr.push_back(buttonHolder());
  
  if(cfg().drawNewUser) bHldr[0].addObj(cfg().newUser);
  bHldr[0].addObj(clearBut);
  bHldr[0].internalSpace(30);
  
  bHldr[1].addObj(undoBut);
  bHldr[1].addObj(redoBut);
  bHldr[1].internalSpace(20);
  
  for (unsigned int i=0; i<sets.size(); i++) sets(i).w=sets(i).h=72;
  
  for (unsigned int i=0; i<sets.size(); i++) bHldr[2].addObj(sets(i));
  bHldr[2].internalSpace(20);
  
  double maxH=0;
  int pad=ofGetWidth();
  
  for (unsigned int i=0; i<bHldr.size(); i++) {
    maxH=max(maxH,bHldr[i].maxHeight());
    pad-=bHldr[i].area.width;
  }
  buttonBar=ofRectangle(x,y,ofGetWidth(),maxH*1.2);
  
  for (unsigned int i=0; i<bHldr.size(); i++) {
    bHldr[i].area.height=bHldr[i].h=buttonBar.height;
    bHldr[i].padding(pad/bHldr.size());
  }
  
  h=subBar.height+buttonBar.height;
  w=ofGetWidth();
}

void controlBar::setup(bGroup * bG, sbGroup * sbG)
{
	blocks=bG;
	sideBar=sbG;

	serChk.setup();
  
	report().setup();
  
	clearBut.setup(cfg().clearMsg, cfg().buttonFontSize);

	if(cfg().saveNotUndo){
		redoBut.setup(50, OF_VERT, "images/load.png");
		undoBut.setup(50, OF_VERT, "images/save.png");
		setAvailableButtons();
	}
	else{
		redoBut.setup(64, OF_VERT, "images/redo.png","images/redo_active.png");
		undoBut.setup(64, OF_VERT, "images/undo.png","images/undo_active.png");
	}
	demo.setup(cfg().demoMsg, cfg().buttonFontSize);
	skipBut.setup(300, 100, "images/skipBut.png");

	anim.setup(blocks, sideBar);
  
	subtitle.loadFont("fonts/DinC.ttf");
	subtitle.setSize(22);
	subtitle.setMode(OF_FONT_TOP);
  
	subBar.height=subtitle.stringHeight("Kjhg")*1.5;
	subBar.width=ofGetWidth();
  
	//ROOT_DIR=config("robots/config.cfg");
    cout << cfg().robotRoot << " is the root" << endl;
	sets.load(cfg().robotRoot);
    cout << sets.size() << " sets" << endl;
	loadBlocks(sets[0]);

  
	sets[0].choice.setAvailable(false);
  
	upload.setup(blocks,&serChk);
  
  if(cfg().test)
	test().setup(&blocks->base);
  
  barSpacing();
  
  loadBlocks(sets[0]);
  setAvailableButtons();
  
  create.setup(cfg().createMsg, 35);
  edit.setup(cfg().editMsg,35);
  
  int maxW=max(create.w,edit.w);
  
  create.w=edit.w=maxW;
  
  bChooseLevel=bPluginChoice=false;
}

void controlBar::loadBlocks(blockGroup & bg){
	
  if(bg.nLoaded>=3){
    
    //--------- load the new blocks with the blockGroup data
	if(cfg().verbose) cout << "The current title is " << bg.title << endl;
	cfg().robotTitle=bg.title;
    sideBar->clear();
    sideBar->setup(bg.blockXML,blocks);
    //anim.changeAnimXML(bg.animXML);
	anim.changeBlockLevel(bg);
  }
}

void controlBar::draw(int _x, int _y)
{
  buttonBar.x=x=_x;
  buttonBar.y=y=_y;
  subBar.x=x;
  subBar.y=y+buttonBar.height;
  
  
  //_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_
  //_-_-_-_-_//_-_-_-_-_//buttonbar//_-_-_-_-_//_-_-_-_-_
  
  if(!cfg().defaultColor) ofSetColor(cfg().controlBarColor);
  else ofSetColor(black);
  ofRect(buttonBar.x,buttonBar.y,buttonBar.width,buttonBar.height);
  
  ofSetColor(gray.opacity(128));
  if(cfg().defaultColor) drawHatching(buttonBar.x, buttonBar.y, buttonBar.width, buttonBar.height, 50,50);
  
  for (unsigned int i=0; i<sets.size(); i++) sets(i).w=sets(i).h=72;
  bHldr[0].draw(buttonBar.x,buttonBar.y);
  for (unsigned int i=1; i<bHldr.size(); i++) {
    bHldr[i].draw(bHldr[i-1].area.x+bHldr[i-1].area.width,buttonBar.y);
  }
  
  //_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_
  //_-_-_-_-_//_-_-_-_-_//subTitle //_-_-_-_-_//_-_-_-_-_
  
  if(cfg().defaultColor) ofSetColor(gray);
  else ofSetColor(cfg().subtitleColor);
  ofRect(subBar.x,subBar.y,subBar.width,subBar.height);
  drawBorder(subBar);
  
  if(cfg().defaultColor) ofSetColor(yellow);
  else ofSetColor(cfg().lineColor);
  ofRect(subBar.x, subBar.y, subBar.width, 1);
  
  if(sets.getSelected()){
    ofButton & t=sets.getSelected()->choice;
    int wid=t.w/16+1;
    if(cfg().defaultColor) ofSetColor(yellow);
	else ofSetColor(cfg().lineColor);
    ofRect(t.x-wid, y, t.w+wid*2, buttonBar.height);
    wid=t.w/16;
	if(cfg().defaultColor) ofSetColor(gray);
	else ofSetColor(cfg().subtitleColor);
    ofRect(t.x-wid, y, t.w+wid*2, buttonBar.height+10);
    t.draw(t.x,t.y);
    
    if(cfg().defaultColor) ofSetColor(yellow);
	else ofSetColor(cfg().textColor);
    subtitle.setSize(22);
    subtitle.setMode(OF_FONT_LEFT);
    subtitle.drawString(sets.getSelected()->subtitle, 50, subBar.y+(subBar.height-subtitle.stringHeight(sets.getSelected()->subtitle))/2+5);
  }
  
  if(cfg().test&&test().isTesting()){
    ofRectangle & cBar=test().controlBar;
    
    ofSetColor(0x33, 0x33, 0x33);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(black);
    drawHatching(0, 0, ofGetWidth(), ofGetHeight(), 15,1);
    
    test().draw(0,cBar.y+cBar.height,ofGetHeight()-(cBar.y+cBar.height),ofGetHeight()-(cBar.y+cBar.height));
    blocks->base.draw(ofGetHeight(), y);
	cfg().test=false;
	blocks->base.drawButtonArea(ofGetHeight(),y);
	cfg().test=true;
    test().drawCurrentBlock();
    
    test().drawControlBar(x, y);
  }
}

void controlBar::drawForeground(){
  
  if(anim.isPlaying()){
    drawStyledBox(skipBut.x-50, skipBut.y-50, skipBut.w+100, skipBut.h+100);
    skipBut.draw((ofGetWidth()-skipBut.w)/2, ofGetHeight()*3./4);
  }
  
  
  if(report().isReporting()){
	  report().draw();
  }
  else if(serChk.drawForeground());
  else if(upload.drawForeground());
  else if(cfg().savePrograms&&bPluginChoice){
	  ofSetColor(black.opacity(200));
	  ofRect(0,0,ofGetWidth(),ofGetHeight());
	  drawStyledBox(create.x-50, create.y-50, create.w+100, (edit.y-create.y)+edit.h+100);
	  create.draw((ofGetWidth()-create.w)/2,(ofGetHeight()-create.h*2)/2-50);
	  edit.draw((ofGetWidth()-edit.w)/2,(ofGetHeight()+edit.h*2)/2+50);
  }
  else if(bChooseLevel){
    //ofSetColor(0, 0, 0,192);
    //ofRect(0, y, ofGetWidth(), ofGetHeight());
    for (unsigned int i=0; i<sets.size(); i++) sets(i).w=sets(i).h=200,sets(i).setAvailable(true);
    
	subtitle.setSize(50);
	subtitle.setMode(OF_FONT_CENTER);
	subtitle.setMode(OF_FONT_TOP);
	string title1="Select a programmer level";
	string title2="to begin.";
	double boxWid=(sets.size()-1)*ofGetWidth()/(sets.size())+(sets(0).w)+100;
	boxWid=max(boxWid,double(subtitle.stringWidth(title1)));

	double stringHgt=subtitle.stringHeight(title1)*2+50;
	double boxHgt=sets(0).h+stringHgt+150;

	drawStyledBox(0, y, ofGetWidth(),ofGetHeight()-y);
    //drawStyledBox((ofGetWidth()-boxWid)/2, ofGetHeight()/2-stringHgt-50, boxWid,boxHgt);
	ofSetColor(cfg().textColor);
	subtitle.drawString(title1,ofGetWidth()/2,ofGetHeight()/2-stringHgt);
	subtitle.drawString(title2,ofGetWidth()/2,ofGetHeight()/2-stringHgt+(stringHgt)/2);
    for (unsigned int i=0; i<sets.size(); i++) sets(i).draw((i+.5)*ofGetWidth()/(sets.size())-(sets(i).w)/2,ofGetHeight()/2+50);
  }
  else if(cfg().test&&test().isTesting()) test().drawForeground();
  else anim.drawForeground();
  
  
  anim.drawCursor();
}

void controlBar::update()
{
  serChk.threadCheckAvailability();
  anim.update();
  
  if(serChk.justLostDevice()){
    if(!bPluginChoice){
      if(blocks->changedSinceSave()) report().post(cfg().disconnectMsg,3);
	  blocks->saveXML(cfg().programDir+serChk.deviceNumber()+".xml");
      blocks->clearAndReset();
      if (cfg().test&&test().isTesting()) {
        test().stopTesting();
        test().resetTurtle();
      }
    }
  }
  if(serChk.justFoundDevice()){
	if(cfg().savePrograms) bPluginChoice=true;
    else {
		beginLevelChoice();
	}
  }
    
  if(timeOut.justExpired()){
	if(cfg().savePrograms) bPluginChoice=true;
	else {
		beginLevelChoice();
	}
      blocks->clearAndReset();
      anim.clearPrompt();
      upload.uploadInBG();
  }
 
  upload.update();
  if(cfg().test&&test().turtleIsRunning()){
    test().idleTurtle();
  }
}

void controlBar::beginLevelChoice(){
	if(sets.size()>1) bChooseLevel=true;
	else if(sets.size()==1){
		if(sets.clickDown(sets[0].choice.x,sets[0].choice.y)&&!anim.isPlaying()){
			if(bChooseLevel){
				bChooseLevel=false;
			}
			if(cfg().demoAvailable) anim.startPrompt();
			if(sets.getSelected())
				loadBlocks((*sets.getSelected()));
		}
		if(cfg().demoAvailable) anim.startPrompt();
	}
}

//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_
//_-_-_-_-_//_-_-_-_-_ mouse func//_-_-_-_-_//_-_-_-_-_

bool controlBar::clickDown(int _x, int _y, int button)
{
	bool ret=false;
  if(!mouseLockout(button)){
    timeOut.set(cfg().timeout);
    timeOut.run();
    if (clearBut.clickDown(_x, _y)) {
      blocks->clearAndReset();
        upload.uploadInBG();
	  ret=true;
    }
    
    //if(demo.clickDown(_x, _y)){
    //  anim.play();
    //}
    
    //--------- if we press the undo button, roll back the state of the blockGroup
    if (undoBut.clickDown(_x, _y)) {
		if(!cfg().saveNotUndo) blocks->undoState();
		else blocks->saveState(),setAvailableButtons();
		ret=true;
    }
    
    //--------- if we press the redo button, push the state forward
    if (redoBut.clickDown(_x, _y)) {
		if(!cfg().saveNotUndo) blocks->redoState();
		else blocks->loadState();
	  ret=true;
    }

	if(cfg().newUser.clickDown(_x,_y)){
		if(cfg().savePrograms) bPluginChoice=true;
		else if(sets.size()!=1) bChooseLevel=true;
		else bChooseLevel=false,anim.startPrompt();
		blocks->clearAndReset();
		blocks->loadFile(ofToDataPath("programs/newUserProgram.xml"));
	}
    
    if(cfg().test) test().clickDown(_x, _y);
  }

  
  //--------- if we press the skip button while the anim is running, stop anim
  if(anim.isPlaying()&&skipBut.clickDown(_x, _y)&&button!=VMOUSE_BUTTON){
    anim.stop();
	ret=true;
  }
  
	if(anim.isPrompting()&&!bChooseLevel){
		if(anim.clickDown(_x, _y)) blocks->recordState();
		ret=true;
	}
  
	if(cfg().test){
		if ((!mouseLockout(button)||test().mouseLockout())||(anim.isPlaying()&&button==VMOUSE_BUTTON)) {
			test().clickDown(_x, _y);
		}
	}
  
  if(bChooseLevel||!mouseLockout(button)){
	  timeOut.set(cfg().timeout);
    if(sets.clickDown(_x,_y)&&!anim.isPlaying()){
		ret=true;
      if(bChooseLevel){
        bChooseLevel=false;
        //anim.play();
        if(cfg().demoAvailable) anim.startPrompt();
      }
      if(cfg().demoAvailable) anim.startPrompt();
      if(sets.getSelected())
        loadBlocks((*sets.getSelected()));
    }
  }

  if((!mouseLockout(button)||(mouseLockout(button)&&test().isTesting()))&&upload.clickDown(_x, _y)){
	  cout << "saving " << cfg().programDir+serChk.deviceNumber()+".xml" << endl;
    blocks->saveXML(cfg().programDir+serChk.deviceNumber()+".xml");
	if(cfg().test){
		test().resetTurtle();
		test().stopTesting();
	}
	ret=true;
  }

  if(cfg().savePrograms&&bPluginChoice){
	  if(create.clickDown(_x,_y)){
		  bPluginChoice=false;
		  bChooseLevel=true;
		  blocks->clearAndReset();
		  blocks->recordState();
		  ret=true;
	  }
	  if(edit.clickDown(_x,_y)){
		  bPluginChoice=false;
		  sysCall.run("dir \""+cfg().programDir+"\"");
		  while(sysCall.isRunning());
		  anim.clearPrompt();
		  for(unsigned int i=0; i<sysCall.outputLines(); i++){
			  vector<string>spl=ofSplitString(sysCall[i]," ");
			  if(spl.size()>=5){
				  if(spl[4]==serChk.deviceNumber()+".xml")
					  blocks->loadFile(cfg().programDir+serChk.deviceNumber()+".xml");
			  }
		  }
		  
		  bChooseLevel=false;
	  }
  }

  return ret;
}

bool controlBar::clickUp()
{
  sets.clickUp();
  clearBut.clickUp();
  demo.clickUp();
  undoBut.clickUp();
  redoBut.clickUp();
  skipBut.clickUp();
  if(cfg().test) test().clickUp();
  cfg().newUser.clickUp();
  edit.clickUp();
  create.clickUp();
  anim.clickUp();
  return false;
}

bool controlBar::mouseLockout(int button)
{
  bool ret=false;
  ret|=(anim.isPlaying()&&button!=VMOUSE_BUTTON);
  ret|=report().isReporting();
  ret|=bPluginChoice;
  ret|=!serChk.isAvailable();
  ret|=bChooseLevel;
  ret|=upload.isUploading();
  ret|=anim.isPrompting();
  if(cfg().test) ret|=test().mouseLockout();
  return ret;
}

void controlBar::setAvailableButtons()
{
	if(!cfg().saveNotUndo){
		redoBut.setAvailable(blocks->redoAvailable());
		undoBut.setAvailable(blocks->undoAvailable());
	}
	else{
		redoBut.setAvailable(blocks->stateAvailable());
	}
	demo.setAvailable(!anim.isPlaying());
}

void controlBar::resize()
{
	bHldr.clear();
  sideBar->updateHeight();
	subBar.width=ofGetWidth();
	barSpacing();
}