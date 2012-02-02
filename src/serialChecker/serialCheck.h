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
#include "RFID_reader.h"

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
  ofSerial serial;
  int nCurrentDevice;
  int numDevices;
  ofTimer checkTimer;
  
  unsigned char		MODE;
	unsigned char		byteRead;
	int			waitForData;
	unsigned char		data[3];
  
  RFIDreader rfid;
  
  vector<serialDevice> devices;
  
  ofFont report;
public:
  serialCheck();
  ~serialCheck();
  bool isAvailable();
  bool justFoundDevice();
  bool justLostDevice();
  void addDevice(string prtNm);
  void deviceRemoved();
  void checkAvailability();
  void threadCheckAvailability();
  bool getDeviceNumber();
  string deviceNumber();
  
  bool drawForeground();
  
  void start(){
    startThread(true, false);   // blocking, verbose
  }
  
  void stop(){
    stopThread();
  }
  void threadedFunction();
};
