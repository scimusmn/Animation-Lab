/*
 *  RFID_reader.h
 *  arenaBot
 *
 *  Created by Exhibits on 11/17/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#pragma once

#include "/Library/Frameworks/Phidget21.framework/Headers/phidget21.h"

#include "ofMain.h"
#include "ofExtended.h"

class RFIDreader {
protected:
  CPhidgetRFIDHandle rfid;
public:
  RFIDreader();
  ~RFIDreader();
  bool available();
  string getSerialNumber();
};