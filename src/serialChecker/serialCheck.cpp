/*
 *  serialCheck.cpp
 *  arenaBot
 *
 *  Created by Exhibits on 11/2/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "serialCheck.h"
#include "dallasEng.h"

int CURRENT_ROBOT_NUMBER=0;

//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_
//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_ SerialDev //_-_-_-_-_//_-_-_-_-_//_-_-_-_-_

serialDevice::serialDevice(string prtNm)
{
  portName=prtNm;
  
#if defined( TARGET_OSX )
  if(prtNm.substr(0,7) != "tty.usb" ) type=NOT_USB_SERIAL;
  else if( prtNm.substr(0,8) != "tty.usbm" ) type=ARD_UNO;
  else if( prtNm.substr(0,8) != "tty.usbs" ) type=ARD_DUEM;
#elif defined( TARGET_WIN32 )
  
#endif
}

//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_
//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_ SerialChk //_-_-_-_-_//_-_-_-_-_//_-_-_-_-_

serialCheck::serialCheck()
{
  serial.setVerbose(false);
  //ofSetLogLevel(OF_LOG_NOTICE);
  numDevices=0;
  checkTimer.set(1);
  checkTimer.run();
  bAvailable=bJustFound=bJustLost=false;
  MODE=0;
	waitForData=2;
	data[0]=data[1]=data[2]=0;
  
  report.loadFont("fonts/DinC.ttf");
  report.setMode(OF_FONT_CENTER);
  report.setMode(OF_FONT_MID);
  report.setSize(70);
  
  //start();
}

serialCheck::~serialCheck(){
  stop();
  devices.clear();
}

bool serialCheck::isAvailable()
{
  bool ret=0;
  if(lock()){
    ret=bAvailable;//&&rfid.available();
    unlock();
  }
  return ret;
}

bool serialCheck::drawForeground()
{
  bool ret=0;
  if(!isAvailable()){
    ofSetColor(0, 0, 0,196);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    string printOut="Connect the robot to begin";
    //if(bAvailable) printOut="Place robot on platform";
    //if(bIdent) printOut="Identifying: Do not unplug";
    
    drawStyledBox((ofGetWidth()-report.stringWidth(printOut))/2-50, (ofGetHeight()-report.stringHeight(printOut))/2-50, report.stringWidth(printOut)+100, report.stringHeight(printOut)+100);
    
    ofSetColor(255, 255, 255);
    report.drawString(printOut, ofGetWidth()/2, ofGetHeight()/2);
    ret=1;
  }
  return ret;
}

void serialCheck::checkAvailability()
{
  if(checkTimer.expired()){
    serial.enumerateDevices();
    if(serial.numDevices()!=numDevices){
      if(numDevices<serial.numDevices()&&!bAvailable){
        numDevices=serial.numDevices();
        cout << "Checking for robots:\n";
  #if defined( TARGET_OSX )
        for (int i=0; i<serial.numDevices(); i++) {
          if(serial.deviceNameByNumber(i).substr(0,7) == "tty.usb" ){
            bAvailable=true;
            bJustFound=true;
          }
        }
  #elif defined( TARGET_WIN32 )
        for (int i=0; i<serial.numDevices(); i++) {
          serial.setup(serial.deviceNameByNumber(i), 9600);
          if(getDeviceNumber()){
            lock();
            bAvailable=true;
            bJustFound=true;
            unlock();
          }
          serial.close();
        }
  #endif
      
      }
      else if(numDevices>serial.numDevices()&&bAvailable){
        nCurrentDevice=-1;
        numDevices=serial.numDevices();
        bAvailable=false;
        bJustLost=true;
        //for (int i=0; i<serial.numDevices(); i++)
          //cout << serial.deviceNameByNumber(i) << endl;
      }
      else if(numDevices>serial.numDevices()&&!bAvailable){
        nCurrentDevice=-1;
        numDevices=serial.numDevices();
        bAvailable=false;
        bJustLost=true;
        //for (int i=0; i<serial.numDevices(); i++)
          //cout << serial.deviceNameByNumber(i) << endl;
      }
    }
    checkTimer.reset();
  }
}

bool serialCheck::getDeviceNumber()
{
  
}

void serialCheck::addDevice(string prtNm)
{
  devices.push_back(serialDevice(prtNm));
}

void serialCheck::deviceRemoved()
{
  
}

string serialCheck::deviceNumber()
{
  return "default";//rfid.getSerialNumber();
}

void serialCheck::threadCheckAvailability()
{
  if (checkTimer.expired()) {
    start();
  }
}

bool serialCheck::justFoundDevice()
{
  bool ret=bJustFound&&isAvailable();//&&rfid.available();
  bJustFound=false;
  return ret;
}

bool serialCheck::justLostDevice()
{
  bool ret=bJustLost;
  bJustLost=false;
  return ret;
}

void serialCheck::threadedFunction(){
  
  //while( isThreadRunning() != 0 ){
    //if( lock() ){
    checkAvailability();
    stop();
    //  unlock();
    //}
  //}
}
