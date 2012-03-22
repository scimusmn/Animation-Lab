/*
 *  uploadModule.cpp
 *  arenaBot
 *
 *  Created by Exhibits on 11/10/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "uploadModule.h"

extern ofColor gray,orange,red,white;

void uploadModule::setup(bGroup * blks, serialCheck * srCk)
{
	serChk=srCk;
  blocks=blks;
  label.loadFont("fonts/DinC.ttf");
  label.setSize(70);
  label.setMode(OF_FONT_CENTER);
  uploaded.pause();
  cmplr.setup(serChk);
}

void uploadModule::update()
{
	cmplr.update();
}

void uploadModule::upload()
{
  //--------- Generate the file which we will compile and upload to the arduino
  blocks->writeFile("arduino_make/applet/arduino_make.cpp");
  
  cmplr.compile("arduino_make/applet/arduino_make.cpp");
  //--------- once we've generated the file, compile and upload with one of teh following scripts
  
  //--------- unpress the upload button
  blocks->base.uploadBut.clickUp();
}

bool uploadModule::drawForeground()
{
  bool ret=0;
  if (isUploading()) {
    ofSetColor(0, 0, 0,192);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
		ofSetColor(255, 255, 255);
		spinner.draw((ofGetWidth())/2,ofGetHeight()/2, 300);
    ofSetColor(white);
		label.drawString("Uploading: do not unplug", ofGetWidth()/2, ofGetHeight()/4);
    ofSetColor(gray);
    ofRect(ofGetWidth()/4, ofGetHeight()*7/8., ofGetWidth()/2, 30);
    ofSetColor(orange);
    ofRect(ofGetWidth()/4+5, ofGetHeight()*7/8.+5, (ofGetWidth()/2-10)*cmplr.percentDone(), 20);
    ret=true;
    label.setSize(20);
    label.drawString(cmplr.report(), ofGetWidth()/2, ofGetHeight()*7/8.+60);
    label.setSize(70);
	}
  else if(0&&uploaded.running()){
    ret=true;
    ofSetColor(0, 0, 0,192);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofRectangle k=label.getBoundingBox("You may now unplug the robot.", ofGetWidth()/2, ofGetHeight()/2);
    drawStyledBox(k.x-50, k.y-50, k.width+100, k.height+100);
    ofSetColor(white);
		label.drawString("You may now unplug the robot.", ofGetWidth()/2, ofGetHeight()/2);
  }
  return ret;
}

void uploadModule::stopUpload(){
  //command.stop();
}

bool uploadModule::isUploading()
{
  if(!cmplr.isCompiling()&&bRunning){
    uploaded.set(3);
    uploaded.run();
  }
  bRunning=cmplr.isCompiling();
  return bRunning;
}

bool uploadModule::clickDown(int _x, int _y)
{
  bool ret=0;
  if(blocks->base.uploadBut.clickDown(_x, _y)&&!isUploading()){
    ret=true;
    upload();
  }
  return ret;
}

