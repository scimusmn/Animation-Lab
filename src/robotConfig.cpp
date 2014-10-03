#include "robotConfig.h"

static configuration config;

configuration & cfg(){
	return config;
};

void configuration::readGeneralConfig(string cfgFile){
    cout << ofToDataPath(cfgFile) << endl;
	ifstream config(ofToDataPath(cfgFile).c_str());
	while (config.peek()!=EOF) {
		string nextLine;
		getline(config, nextLine);
		vector<string> token=ofSplitString(nextLine, "=");
		if(token.size()){
            cout << token[0] << endl;
			if(token[0]=="MCU") mcu=token[1];
			else if(token[0]=="TRANSFER_RATE") baud=token[1];
			else if(token[0]=="PROGRAMMER") programmer=token[1];
			else if(token[0]=="F_CPU"){
				if(token[1]=="16M") freq="16000000L";
				else if(token[1]=="8M") freq="8000000L";
				else if(token[1]=="20M") freq="20000000L";
				else freq=token[1];
			}
			else if(token[0]=="BOARD"){
				if(token[1]=="UNO") mcu="atmega328p", programmer="arduino", freq="16000000L",baud="115200";
				else if(token[1]=="DUEM") mcu="atmega328p", programmer="arduino", freq="16000000L",baud="57600";
                else if(token[1]=="PEAPOD") mcu="attiny85", programmer="usbtiny", freq="8000000L",baud="";
			}
			else if(token[0]=="EXCLUDE_PORT"){
				excludedPort.push_back(token[1]);
				if(verbose) cout << "Port " << token[1] << " will not be used to program."<<endl;
			}
			else if(token[0]=="INSTALL_DIR"){
				rootDir=ofToDataPath(token[1]);
				toolDir=rootDir+"hardware/tools/avr/bin/";
			}
			else if(token[0]=="PERIPHERAL"){
				addonLib=ofToDataPath(token[1]);
			}
			else if(token[0]=="VERBOSE"){
				verbose=ofToInt(token[1]);
			}
			else if(token[0]=="TIMEOUT"){
				timeout=ofToInt(token[1]);
                cout << "timeout is " << timeout/1000 << "seconds" << endl;
			}
			else if(token[0]=="ROBOT"){
				if(verbose) cout << token[1] << " is the current robot configuration." << endl;
				robotRoot=ofToDataPath("robots/"+token[1]);
			}
			else if(token[0]=="TEST_AVAILABLE"){
				test=ofToInt(token[1]);
			}
			else if(token[0]=="SCROLL_SCREEN"){
				scroll=ofToInt(token[1]);
			}
			else if(token[0]=="PORTRAIT_MODE"){
				portraitMode=ofToInt(token[1]);
				//ofSetWindowShape(ofGetScreenWidth(),ofGetScreenHeight());
			}
			else if(token[0]=="NEW_USER_BUTTON"){
				if(token[1].length()>1){
					drawNewUser=true;
					//newUser.setTextSize(ofToInt(token[1]));
					newUser.setup(token[1],buttonFontSize);
				}
				else drawNewUser=false;
			}
			else if(token[0]=="WRAPPER"){
				wrapperFile=token[1];
			}
			else if(token[0]=="BLOCK_FONT_SIZE"){
				blockFontSize=ofToInt(token[1]);
			}
			else if(token[0]=="BUTTON_FONT_SIZE"){
				buttonFontSize=ofToInt(token[1]);
			}
			else if(token[0]=="UPLOADED_MESSAGE"){
				uploadedMessage=token[1];
			}
			else if(token[0]=="CONNECT_MESSAGE"){
				connectMessage=token[1];
			}
			else if(token[0]=="DISCONNECT_MESSAGE"){
				disconnectMsg=token[1];
			}
			else if(token[0]=="DEMO_MESSAGE"){
				demoMsg=token[1];
			}
			else if(token[0]=="CREATE_MESSAGE"){
				createMsg=token[1];
			}
			else if(token[0]=="EDIT_MESSAGE"){
				editMsg=token[1];
			}
			else if(token[0]=="CLEAR_MESSAGE"){
				clearMsg=token[1];
			}
			else if(token[0]=="BOARD_DETECT"){
			    boardDetect=ofToInt(token[1]);
			}
			else if(token[0]=="LEVEL_FOLDER"){
		      levelFolders.push_back(token[1]);
			}
			else if(token[0]=="ABS_SAVE_DIR"){
				programDir=token[1];
				if(verbose) cout << "The directory to which programs are saved is " << programDir <<endl;
			}
			else if(token[0]=="SAVED_PROGRAM_DIR"){
				programDir=ofToDataPath(token[1]);
			}
			else if(token[0]=="SAVE_PROGRAMS"){
				savePrograms=ofToInt(token[1]);
			}
			else if(token[0]=="FULLSCREEN"){
				ofSetFullscreen(ofToInt(token[1]));
			}
			else if(token[0]=="TITLE_BAR_SIZE"){
				titleBarSize=ofToInt(token[1]);
                if(titleBarSize) showTitle=true;
			}
			else if(token[0]=="DEMO_AVAILABLE"){
				demoAvailable=ofToInt(token[1]);
			}
			else if(token[0]=="CONTROL_BAR_COLOR"){
				controlBarColor=ofColor(strtol(token[1].c_str(),NULL,0));
				controlBarColor.a=255;
			}
			else if(token[0]=="BG_COLOR"){
				backgroundColor=ofColor(strtol(token[1].c_str(),NULL,0));
				backgroundColor.a=255;
			}
			else if(token[0]=="SIDE_BAR_COLOR"){
				sideBarColor=ofColor(strtol(token[1].c_str(),NULL,0));
				sideBarColor.a=255;
			}
			else if(token[0]=="LINE_COLOR"){
				lineColor=ofColor(strtol(token[1].c_str(),NULL,0));
				lineColor.a=255;
			}
			else if(token[0]=="TEXT_COLOR"){
				textColor=ofColor(strtol(token[1].c_str(),NULL,0));
				textColor.a=255;
			}
			else if(token[0]=="SUBTITLE_COLOR"){
				subtitleColor=ofColor::fromHex(strtol(token[1].c_str(),NULL,0));
                cout << subtitleColor << endl;
				subtitleColor.a=255;
			}
			else if(token[0]=="DEFAULT_COLORS"){
				defaultColor=ofToInt(token[1]);
			}
			else if(token[0]=="BUTTONS_ON_SIDEBAR"){
				buttonsOnSidebar=ofToInt(token[1]);
			}
			else if(token[0]=="SAVE_NOT_UNDO"){
				saveNotUndo=ofToInt(token[1]);
			}
		}
	}
	config.close();
    if(!wrapperFile.length()) wrapperFile = robotRoot+"/wrapper.wrp";
}