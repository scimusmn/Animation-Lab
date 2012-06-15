#include "robotConfig.h"

static configuration config;

configuration & cfg(){
	return config;
};

void configuration::readGeneralConfig(string cfgFile){
	ifstream config(ofToDataPath(cfgFile).c_str());
	while (config.peek()!=EOF) {
		string nextLine;
		getline(config, nextLine);
		vector<string> token=ofSplitString(nextLine, "=\r\n");
		if(token.size()){
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
			}
			else if(token[0]=="EXCLUDE_PORT"){
				excludedPort.push_back(token[1]);
				cout << token[1] << " will be excluded"<<endl;
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
			}
			else if(token[0]=="ROBOT"){
				cout << token[1] << endl;
				robotRoot="robots/"+token[1];
			}
			else if(token[0]=="TEST_AVAILABLE"){
				test=ofToInt(token[1]);
			}
			else if(token[0]=="SCROLL_SCREEN"){
				scroll=ofToInt(token[1]);
			}
			else if(token[0]=="PORTRAIT_MODE"){
				portraitMode=ofToInt(token[1]);
				ofSetWindowShape(ofGetScreenWidth(),ofGetScreenHeight());
			}
			else if(token[0]=="DRAW_NEW_USER_BUTTON"){
				drawNewUser=ofToInt(token[1]);
				//newUser.setTextSize(ofToInt(token[1]));
				newUser.setup("New user?",23);
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
      else if(token[0]=="BOARD_DETECT"){
        boardDetect=ofToInt(token[1]);
      }
		}
	}
	config.close();
}