#pragma once

/*
*
* Trying out a new coinfiguration reader, for globalized constants
*
*/

#include "ofMain.h"
#include "ofExtended.h"
#include "../../dallasEng/dallasEng.h"

struct configuration {
	string mcu, baud, programmer,freq,wrapperFile; // compiler vars
	vector<string> excludedPort;
	string rootDir,toolDir,addonLib; // directory vars
	double timeout;
	bool verbose;
	bool test;
	bool scroll;
	bool portraitMode;
	dallasButton newUser;
	bool drawNewUser;
	int buttonFontSize;
	int blockFontSize;
  bool boardDetect;
	string robotRoot,robotTitle;

	string uploadedMessage;
	void readGeneralConfig(string cfgFile);
	configuration(){
		drawNewUser=scroll=test=verbose=portraitMode=false;
		ofEnableDataPath();
	}
	void setup(){
		readGeneralConfig("config.txt");
	}
};

configuration & cfg();