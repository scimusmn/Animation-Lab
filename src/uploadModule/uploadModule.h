/*
 *  uploadModule.h
 *  arenaBot
 *
 *  Created by Exhibits on 11/10/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#pragma once

#include "../blockGroup/blockGroup.h"
#include "../compiler/compiler.h"
#include "../serialChecker/serialCheck.h"

class uploadModule {
protected:
  bGroup * blocks;
  serialCheck * serChk;
	ofProgressSpinner spinner;
  ofFont label;
  bool bRunning;
  ofTimer uploaded;
    bool bgUpload;

  compiler cmplr;
public:
  void setup(bGroup * blks, serialCheck * srCk);
  void upload();
    void uploadInBG();
  void update();
  bool drawForeground();
  bool isUploading();
  void stopUpload();
  bool clickDown(int _x, int _y);
};