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
	string 
		// compiler vars
		mcu, 
		baud, 
		programmer,
		freq,
		wrapperFile,
		//Directory vars
		rootDir,
		toolDir,
		addonLib,
		programDir,
		//RobotVars
		robotRoot,
		robotTitle,
		//Messages
		uploadedMessage,
		connectMessage,
		disconnectMsg,
		demoMsg,
		createMsg,
		editMsg,
		clearMsg;
	
	vector<string> 
		excludedPort,
		levelFolders;

	double timeout;
	dallasButton newUser;
	int 
		buttonFontSize,
		blockFontSize,
		titleBarSize;

	bool 
		absoluteSaveDir,
		defaultColor,
		showTitle,
		demoAvailable,
		boardDetect,
		drawNewUser,
		verbose,
		test,
		scroll,
		portraitMode,
		savePrograms,
		buttonsOnSidebar;

	ofColor 
		controlBarColor,
		backgroundColor,
		sideBarColor;

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