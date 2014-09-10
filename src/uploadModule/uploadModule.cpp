/*
 *  uploadModule.cpp
 *  arenaBot
 *
 *  Created by Exhibits on 11/10/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "uploadModule.h"
#include "../robotConfig.h"
#include "../screenReport/screenReport.h"

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
  bRunning=false;
    bgUpload=false;
}

void uploadModule::update()
{
	cmplr.update();
}

void uploadModule::upload()
{
	if(blocks->base.blocksOn.size()){
		//--------- Generate the file which we will compile and upload to the arduino
		blocks->writeFile("arduino_make/applet/arduino_make.cpp");
  
		cmplr.compile("arduino_make/applet/arduino_make.cpp");
	}
	else report().post("Connect blocks to the base\nto program",3);
	//--------- unpress the upload button
		
	blocks->base.uploadBut.clickUp();
}

void uploadModule::uploadInBG()
{
    if(!bRunning){
        bgUpload=true;
        //--------- Generate the file which we will compile and upload to the arduino
        //blocks->writeFile("arduino_make/applet/arduino_make.cpp");
         cmplr.compile("arduino_make/applet/blank.cpp");
    }
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
	if(cfg().test) label.drawString("Uploading: do not unplug", ofGetWidth()/2, ofGetHeight()/4);
	else label.drawString("Uploading...", ofGetWidth()/2, ofGetHeight()/4);
    ofSetColor(gray);
    ofRect(ofGetWidth()/4, ofGetHeight()*7/8., ofGetWidth()/2, 30);
    ofSetColor(orange);
    ofRect(ofGetWidth()/4+5, ofGetHeight()*7/8.+5, (ofGetWidth()/2-10)*cmplr.percentDone(), 20);
    ret=true;
    label.setSize(20);
    label.drawString(cmplr.report(), ofGetWidth()/2, ofGetHeight()*7/8.+60);
    label.setSize(70);
	}
  /*else if(uploaded.running()){
	string message;
	if(cfg().test) message="You may now unplug the robot.";
	else message="Watch the robot!";
    ret=true;
    ofSetColor(0, 0, 0,192);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofRectangle k=label.getBoundingBox(message, ofGetWidth()/2, ofGetHeight()/2);
    drawStyledBox(k.x-50, k.y-50, k.width+100, k.height+100);
    ofSetColor(white);
		label.drawString(message, ofGetWidth()/2, ofGetHeight()/2);
  }*/
  return ret;
}

void uploadModule::stopUpload(){
  //command.stop();
}

bool uploadModule::isUploading()
{
	if(!cmplr.isCompiling()&&bRunning&&!bgUpload){
    //uploaded.set(3);
    //uploaded.run();
		report().post(cfg().uploadedMessage,3);
	}
    else if(!cmplr.isCompiling()&&bRunning) bgUpload=false;
  bRunning=cmplr.isCompiling();
  return bRunning&&!bgUpload;
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

