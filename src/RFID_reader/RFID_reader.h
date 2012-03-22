/*
 *  RFID_reader.h
 *  arenaBot
 *
 *  Created by Exhibits on 11/17/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofExtended.h"

#if defined( TARGET_OSX )
#include "/Library/Frameworks/Phidget21.framework/Headers/phidget21.h"
#else
#include "phidgets/phidget21.h"
#endif

class RFIDreader {
protected:
  CPhidgetRFIDHandle rfid;

public:
  RFIDreader();
  void setup();
  ~RFIDreader();
  bool available();
  string getSerialNumber();
};

RFIDreader & rfid();