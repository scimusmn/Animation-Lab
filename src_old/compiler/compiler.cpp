#include "compiler.h"

void compile_config::load(string cfgFile)
{
	//cFilename=cfgFile;
	ifstream config(ofToDataPath(cfgFile).c_str());
	while (config.peek()!=EOF) {
		string nextLine;
		getline(config, nextLine);
		vector<string> token=ofSplitString(nextLine, "=");
		if(token.size()){
			if(token[0]=="MCU") mcu=token[1];
			if(token[0]=="TRANSFER_RATE") baud=token[1];
			else if(token[0]=="PROGRAMMER") programmer=token[1];
			else if(token[0]=="F_CPU"){
				if(token[1]=="16M") freq="16000000L";
				else if(token[1]=="8M") freq="8000000L";
				else if(token[1]=="20M") freq="20000000L";
				else freq=token[1];
			}
			else if(token[0]=="BOARD"){
				if(token[1]=="UNO") mcu="atmega328p", programmer="arduino", freq="16000000L",baud="115200";
				else if(token[1]=="DUEM") mcu="atmega328p", programmer="stk500v1", freq="16000000L",baud="57600";
			}
			else if(token[0]=="INSTALL_DIR");
			else if(token[0]=="PERIPHERAL"){
#if defined TARGET_OSX
				addonLib=token[1];
#else
				addonLib=de_UnixPath(token[1]);
#endif
			}
		}
	}
	config.close();
}

void command::addArgument(string arg)
{
	args.push_back(arg);
}

void command::newCommand()
{
	args.clear();
}

void command::execute()
{
	string com="";
	for(unsigned int i=0; i<args.size(); i++){
		if(i==0) com+="\"";
		com+=args[i];
		if(i==0) com+="\"";
		if(i<args.size()-1) com+=" ";
	}
#ifdef TARGET_WIN32
	com=de_UnixPath(com);
#endif
	//cout << com << " is the command being run\n";
	bExecuting=true;
	call.run(com);
}

bool command::isExecuting()
{
	return call.isRunning();
}

bool command::justExecuted()
{
	bool ret=0;
	ret=bExecuting&&!call.isRunning();
	bExecuting=call.isRunning();
	return ret;
}

int command::linesOfOutput()
{
	return call.outputLines();
}

string command::operator[](int i)
{
	return call[i];
}

string command::lastLine()
{
	string ret="";
	if(call.outputLines()) ret=call.commandOutput();
	return ret;
}

compiler::compiler()
{
	configure("arduino_make/config.txt");
#ifdef TARGET_OSX
	rootDir=ofToDataPath("../Arduino/");
#else
	rootDir=ofToDataPath("../Arduino_win32/");
#endif
	toolDir=rootDir+"hardware/tools/avr/bin/";

	addonLib=conf.addonLib;
	mode=WAIT;

}

void compiler::update()
{
	if(cmd.justExecuted()){
		switch(mode){
			case COMPILE:
				assemble();
				rep="Assembling...";
				percent=.1;
				break;
			case ASSEMBLE_ELF:
				assemble();
				rep="Making .hex...";
				percent=.20;
				break;
			case ASSEMBLE_HEX:
				rep="Contacting Programmer...";
				upload(port);
				percent=.30;
				break;
			case UPLOAD:
				mode=WAIT;
				rep="Done Uploading.";
				percent=1;
				break;
			default:
				break;
		}
	}
	if(mode==UPLOAD){
		//if(cmd.linesOfOutput()>3&&cmd.linesOfOutput()!=lastLines) cout << cmd[cmd.linesOfOutput()-2] << endl;
		//lastLines=cmd.linesOfOutput();
		int avrdudes=0;
		for(unsigned int i=0; i<cmd.linesOfOutput(); i++){
			if(cmd[i].substr(0,7)=="avrdude") avrdudes++;
		}

		if(avrdudes>=1) rep="Reading chip...",percent=.4;
		if(avrdudes>=3) rep="Erasing chip...",percent=.5;
		if(avrdudes>=5) rep="Writing chip...",percent=.6;
				
		vector<string> spl=ofSplitString(cmd.lastLine()," |");
		if(spl.size()>1){
			if(spl[0]=="Writing"){
				percent=.6+.4*float(spl[1].length())/50.;
			}
		}
		if(avrdudes>=8) rep="Writing chip...",percent=1;
	}
}

void compiler::compile(string filename, string prt)
{
	port=prt;
	file=ofToDataPath(filename);
	fileRoot=file.substr(0,file.find_last_of('.'));
	ifstream base(file.c_str());
	while (base.peek()!=EOF) {
		string nextLine;
		getline(base, nextLine);
		checkForInclude(nextLine);
	}
	base.close();
	includePaths.push_back(rootDir+"hardware/arduino/cores/arduino"); // change this to work with tinyX5;

	cmd.newCommand();
	cmd.addArgument(toolDir+"avr-g++");
	cmd.addArgument("-c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections");
	cmd.addArgument("-DF_CPU="+conf.freq+"L");
	cmd.addArgument("-DARDUINO=22");
	for(unsigned int i=0; i<includePaths.size(); i++){
		cmd.addArgument("-I"+includePaths[i]);
	}
	cmd.addArgument("-mmcu="+conf.mcu);
	cmd.addArgument(fileRoot+".cpp");
	cmd.addArgument("-o " + fileRoot + ".o");

	mode=COMPILE;
	rep="Compiling...";
	percent=0;
	cmd.execute();

	/*
	path/to/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -DF_CPU=processor_speed
	-I(includes_for_cpp) -mmcu=processor_name path/to/target.cpp -o target.o
	*/
}

//"data\..\Arduino_win32\hardware\tools\avr\bin\avrdude" -V -F -C "data\..\Arduino_win32\hardware\tools\avr\bin\..\etc\avrdude.conf" -p atmega328p -P COM7 -c stk500v1 -b 57600 -U flash:w:data\arduino_make\applet\arduino_make.hex
void compiler::assemble()
{
	if(mode==COMPILE){
		mode=ASSEMBLE_ELF;
		cmd.newCommand();
		cmd.addArgument(toolDir+"avr-gcc");
		cmd.addArgument("-Os -lm -Wl,--gc-sections");
		cmd.addArgument("-mmcu="+conf.mcu);
		cmd.addArgument("-o "+fileRoot+".elf");
		cmd.addArgument(fileRoot+".o");
		cmd.addArgument(fileRoot.substr(0,fileRoot.find_last_of("/"))+"/core.a");

		cmd.execute();
	}

	else if(mode==ASSEMBLE_ELF){
		mode=ASSEMBLE_HEX;
		cmd.newCommand();
		cmd.addArgument(toolDir+"avr-objcopy");
		cmd.addArgument("-O ihex -R .eeprom");
		cmd.addArgument(fileRoot+".elf");
		cmd.addArgument(fileRoot+".hex");

		cmd.execute();
	}

	/*

	To assemble .elf

		path/to/avr-gcc -Os -lm-Wl,--gc-sections -o path/to/target.elf path/to/target.o path/to/core.a

	To assemble .hex

		path/to/avr-objcopy -O ihex -R .eeprom path/to/target.elf path/to/target.hex

	*/
}

//"data\..\Arduino_win32\hardware\tools\avr\bin\avrdude" -V -F -C "data\..\Arduino_win32\tool/avr/etc/avrdude.conf" -p atmega328p -P COM7 -c stk500v1 -b 57600 -U flash:w:data\arduino_make/applet/arduino_make.hex

void compiler::upload(string port)
{
	mode=UPLOAD;
	cmd.newCommand();
	cmd.addArgument(toolDir+"avrdude.exe");
	cmd.addArgument("-V -F -C");
	cmd.addArgument(toolDir+"../etc/avrdude.conf");
	cmd.addArgument("-p "+conf.mcu);
	cmd.addArgument("-P "+port);
	cmd.addArgument("-c "+conf.programmer);
	cmd.addArgument("-b "+conf.baud);
	cmd.addArgument("-U flash:w:"+fileRoot+".hex");

	cmd.execute();

	/*
	To upload to the mcu

	path/to/avrdude -V -F -C path/to/avrdude.conf -p mcu_name -P port_name -c programmer_name
		-b transfer_rate -U flash:w:path/to/target.hex
	*/
}

void compiler::checkForInclude(string & line)
{
	for(unsigned int i=0; i<line.length(); i++){
		if(line[i]=='#'){
			vector<string> spl=ofSplitString(line," #<>\"");
			if(spl.size()>1&&spl[0]=="include"){
				//includeFiles.push_back(spl[1]);
				findIncludeFolder(spl[1]);
				//cout << spl[1] << endl;
			}
		}
	}
}

void compiler::findIncludeFolder(string & line)
{
	int nDir=0;
	string inc = line.substr(0,line.find_last_of('.'));
	vector<string> spl = ofSplitString(line,".");
	nDir=dir.listDir(rootDir+"libraries");
	bool found=false;
	for(int i = 0; i < nDir; i++){
		if(dir.getName(i)==spl[0]){
			found=true;
			includePaths.push_back(dir.getPath(i));
			ofxDirList dir2;
			int nDir2=dir2.listDir(dir.getPath(i));
			for(int j=0; j<nDir2; j++){
				if(dir2.getName(j)=="utility") includePaths.push_back(dir2.getPath(j));
			}
		}
	}
	if(!found){
		nDir=dir.listDir(addonLib);
		for(int i = 0; i < nDir; i++){
			if(dir.getName(i)==spl[0]){
				found=true;
				includePaths.push_back(dir.getPath(i));
				ofxDirList dir2;
				int nDir2=dir2.listDir(dir.getPath(i));
				for(int j=0; j<nDir2; j++){
					if(dir2.getName(i)=="utility") includePaths.push_back(dir2.getPath(j));
				}
			}
		}
	}
	if(!found) includePaths.push_back(addonLib);
}
	
void compiler::configure(string cfgFile)
{
	conf.load(cfgFile);
}

bool compiler::isCompiling()
{
	return (mode>COMPILER_ERROR);
}
	
string compiler::report()
{
	return rep;
}

float compiler::percentDone()
{
	return percent;
}
/*** Process to upload to the controller

To compile from the .cpp file:

	path/to/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -DF_CPU=processor_speed
		-I(includes_for_cpp) -mmcu=processor_name path/to/target.cpp -o target.o

To assemble .elf

	path/to/avr-gcc -Os -lm-Wl,--gc-sections -o path/to/target.elf path/to/target.o path/to/core.a

To assemble .hex

	path/to/avr-objcopy -O ihex -R .eeprom path/to/target.elf path/to/target.hex

To upload to the mcu

	path/to/avrdude -V -F -C path/to/avrdude.conf -p mcu_name -P port_name -c programmer_name
		-b transfer_rate -U flash:w:path/to/target.hex

*/