/*
 *  serialCheck.cpp
 *  arenaBot
 *
 *  Created by Exhibits on 11/2/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "serialCheck.h"
#include "../../../dallasEng/dallasEng.h"
#include "../robotConfig.h"

#include "../RFID_reader/RFID_reader.h"

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
  report.setSize(60);
  
  bIdent=false;
  bRunning=false;
  setUp=false;
  //start();
}

void serialCheck::setup(){
  if(cfg().boardDetect){
    rfid().setup();
    setUp=true;
    for(unsigned int i=0; i<cfg().excludedPort.size(); i++){
      excludeDevice(cfg().excludedPort[i]);
    }
  }
}

serialCheck::~serialCheck(){
  stop();
  devices.clear();
}

bool serialCheck::isAvailable()
{
  bool ret=0;
  if(cfg().boardDetect){
    if(lock()){
      ret=bAvailable;
    if(cfg().test) ret&=rfid().available();
      unlock();
    }
  }
  else ret=true;
  return ret;
}

bool serialCheck::drawForeground()
{
  bool ret=0;
  if(!isAvailable()){
    ofSetColor(0, 0, 0,196);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
	string printOut=cfg().connectMessage;
    if(bAvailable&&cfg().test) printOut="Place robot on platform";
    if(bIdent&&cfg().test) printOut="Identifying: Do not unplug";
    
    drawStyledBox((ofGetWidth()-report.stringWidth(printOut))/2-50, (ofGetHeight()-report.stringHeight(printOut))/2-50, report.stringWidth(printOut)+100, report.stringHeight(printOut)+100);
    
    ofSetColor(255, 255, 255);
    report.drawString(printOut, ofGetWidth()/2, ofGetHeight()/2);
    ret=1;
  }
  return ret;
}

string serialCheck::portName()
{
	return portNm;
}

void serialCheck::excludeDevice(string prtNm)
{
	excludedPorts.push_back(prtNm);
}

void serialCheck::checkAvailability()
{
    //cout<< "checking..."<<endl;
  if(checkTimer.expired()&&setUp){
    serial.enumerateDevices();
    if(serial.numDevices()!=numDevices){
      if(numDevices<serial.numDevices()&&!bAvailable){
        numDevices=serial.numDevices();
  #if defined( TARGET_OSX )
        for (int i=0; i<serial.numDevices(); i++) {
            cout<<serial.deviceNameByNumber(i)<< endl;
          bool excluded=false;
          for(unsigned int j=0; j<excludedPorts.size(); j++){
            if(excludedPorts[j]==serial.deviceNameByNumber(i))
              excluded=true;
			  if(cfg().verbose) cout << serial.deviceNameByNumber(i) << " has been excluded\n";
          }
          if((!excluded||excludedPorts.size()==0)&&serial.deviceNameByNumber(i).substr(0,7)=="tty.usb"){
            portNm="/dev/"+serial.deviceNameByNumber(i);
            cout << "Robot is on port " << portNm << endl;
            bAvailable=true;
            bJustFound=true;
          }
        }
  #elif defined( TARGET_WIN32 )
        for (int i=0; i<serial.numDevices(); i++) {
          bool excluded=false;
          for(unsigned int j=0; j<excludedPorts.size(); j++){
            if(excludedPorts[j]==serial.deviceNameByNumber(i))
              excluded=true;
			  if(cfg().verbose)  cout << serial.deviceNameByNumber(i) << " has been excluded from being a robot.\n";
          }
          if(!excluded||excludedPorts.size()==0){
            portNm=serial.deviceNameByNumber(i);
            if(cfg().verbose) cout << "Found a robot on port " << portNm << endl;
            bAvailable=true;
            bJustFound=true;
          }
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
    checkTimer.set(1);
	checkTimer.run();
	stop();
  }
}

bool serialCheck::getDeviceNumber()
{
  return 0;
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
  return ((cfg().test)?rfid().getSerialNumber():"default");
}

void serialCheck::threadCheckAvailability()
{
  if(cfg().boardDetect){
    if (!bRunning&&checkTimer.expired()) {
      start();
    }
  }
}

bool serialCheck::justFoundDevice()
{
  bool ret=bJustFound&&isAvailable();//&&rfid().available();
  if(cfg().test){
	  ret&=rfid().available();
	  if(ret) bJustFound=false;
  }
  else bJustFound=false;
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
    //stop();
    //  unlock();
    //}
  //}
}