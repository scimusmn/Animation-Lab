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
	string programDir;
	bool savePrograms;
	double timeout;
	bool verbose;
	bool test;
	bool scroll;
	bool portraitMode;
	dallasButton newUser;
	bool drawNewUser;
	int buttonFontSize;
	int blockFontSize;
	bool absoluteSaveDir;
	int titleBarSize;
	bool showTitle;
	bool demoAvailable;

	ofColor controlBarColor;
	ofColor backgroundColor;
	ofColor sideBarColor;
  bool boardDetect;
	string robotRoot,robotTitle;
  vector<string> levelFolders;

	string uploadedMessage,connectMessage,disconnectMsg,demoMsg,createMsg,editMsg,clearMsg;
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