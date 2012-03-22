/*
 *  serialCheck.h
 *  arenaBot
 *
 *  Created by Exhibits on 11/2/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofExtended.h"
#include "ofxDirList.h"

#include "ofxThread.h"

#define IS_ROBOT 0x06
#define ROBOT_NUMBER 0x0C
#define REQUEST_IDENT 0xff

enum deviceType {
  NOT_USB_SERIAL, ARD_UNO, ARD_DUEM
};

enum deviceState {
  UNPLUGGED, CONNECTED, INITTED
};

struct serialDevice {
  int number;
  string portName;
  deviceType type;
  deviceState state;
  serialDevice(string prtNm);
};

class serialCheck : public ofxThread{
protected:
  bool bAvailable;
  bool bJustFound;
  bool bJustLost;
  bool bIdent;
  bool bRunning;
  ofSerial serial;
  int nCurrentDevice;
  int numDevices;
  ofTimer checkTimer;
  
  unsigned char		MODE;
	unsigned char		byteRead;
	int			waitForData;
	unsigned char		data[3];
  
  //RFIDreader rfid;
  
  vector<serialDevice> devices;
  
  vector<string> excludedPorts;

  ofFont report;
  string portNm;
  bool setUp;
public:
  serialCheck();
  ~serialCheck();
  void setup();
  bool isAvailable();
  bool justFoundDevice();
  bool justLostDevice();
  void excludeDevice(string prtNm);
  void addDevice(string prtNm);
  void deviceRemoved();
  void checkAvailability();
  void threadCheckAvailability();
  bool getDeviceNumber();
  string deviceNumber();

  string portName();
  
  bool drawForeground();
  
  void start(){
	  bRunning=true;
    startThread(true, false);   // blocking, verbose
  }
  
  void stop(){
	bRunning=false;
    stopThread();
  }
  void threadedFunction();
};

//serialCheck & serChk();