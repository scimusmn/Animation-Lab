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

#define ENABLE_RFID 1

#if ENABLE_RFID
#if defined( TARGET_OSX )
#include "../phidgets/Headers/phidget21.h"
#else
#include "phidgets/phidget21.h"
#endif
#endif

class RFIDreader {
protected:
#if ENABLE_RFID
  CPhidgetRFIDHandle rfid;
#endif
public:
  RFIDreader();
  void setup();
  ~RFIDreader();
  bool available();
  string getSerialNumber();
};

RFIDreader & rfid();