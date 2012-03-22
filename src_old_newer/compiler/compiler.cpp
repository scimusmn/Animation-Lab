#include "compiler.h"

extern string ROOT_DIR;

void command::addArgument(string arg)
{
	args.push_back(arg);
}

void command::newCommand()
{
	args.clear();
}

void command::execute(bool echo)
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
	if(echo) cout << com << endl;
	//cout << com << " is the command being run\n";
	bExecuting=true;
	call.run(com,echo);
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

bool command::isThereOutput()
{
	return !call.noOutput();
}

///////////////////////////////////////////////////////////////////////
////////////////// compiler beyond this point /////////////////////////

Object::Object()
{
}

Object::Object(string fileNm)
{
	setFilePath(fileNm);
}

void Object::setFilePath(string path)
{
	filePath=ofToDataPath(path);
	baseName=filePath.substr(filePath.find_last_of('/')+1,filePath.find_last_of('.')-(filePath.find_last_of('/')+1));
	rootDir=filePath.substr(0,filePath.find_last_of('/'))+"/";
}

void Object::addInclude(string path)
{
	bool found=false;
	for(unsigned int i=0; i<paths.size(); i++){
		if(paths[i]==path){
			found=true;
			break;
		}
	}
	if(!found) paths.push_back(path);
}

int Object::size()
{
	return paths.size();
}

string Object::operator[](int i)
{
	return paths[i];
}

compiler::compiler()
{
#ifdef TARGET_OSX
	rootDir=ofToDataPath("../Arduino/",true);
#else
	rootDir=ofToDataPath("../Arduino_win32/");
#endif
	toolDir=rootDir+"hardware/tools/avr/bin/";

	//addonLib=conf.addonLib;
	mode=WAIT;

}

void compiler::setup(serialCheck * srChk)
{
	bVerbose=false;
	serChk=srChk;
	configure("arduino_make/config.txt");
	objects.reserve(30);
}

void compiler::update()
{
	if(cmd.justExecuted()||bSkipStep){
		bSkipStep=false;
		switch(mode){
			case COMPILE:
				if(!cmd.isThereOutput()){
					currentObj=0;
					compileDepends();
					currentObj++;
					rep="Assembling...";
					percent=.1;
				}
				else {
					string err;
					for(unsigned int i=0; i<cmd.linesOfOutput(); i++){
						if(cmd[i].find_last_of(":")!=cmd[i].npos){
							err=cmd[i].substr(cmd[i].find_last_of(":"));
						}
					}
					rep="Error Compiling"+err;
				}
				break;
			case COMPILE_OBJS:
				if(currentObj>=objects.size()){
					currentObj=0;
					link();
					rep="Linking...";
					percent=.15;
				}
				else {
					compileDepends();
					currentObj++;
				}
				break;
			case LINK:
				assemble();
				rep="Making .hex...";
				percent=.25;
				break;
			case ASSEMBLE:
				rep="Contacting Programmer...";
				upload(serChk->portName());
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

void compiler::computeDependencies(Object & obj)
{
	ifstream base(obj.filePath.c_str());
	while (base.peek()!=EOF) {
		string nextLine;
		getline(base, nextLine);
		vector<string> spl=ofSplitString(nextLine," #<>\"");
		if(spl.size()>1&&spl[0]=="include"){
      if(bVerbose) cout << "looking for " + spl[1] << endl;
			findIncludeFolder(spl[1], obj);
		}
	}
	base.close();
}

void compiler::findIncludeFolder(string & line,Object & obj)
{
	int nDir=0;
	string inc = line.substr(0,line.find_last_of('.'));
	vector<string> spl = ofSplitString(line,".");
	//nDir=dir.listDir(rootDir+"libraries");
	string folder=searchFolder(spl[0],rootDir+"libraries/",obj);
	if(folder.length()==0) folder=searchFolder(spl[0],addonLib,obj);
}

string compiler::searchFolder(string & incl, string startPosition,Object & obj)
{
	string ret="";
	ofxDirList dir;
	//cout << startPosition << " is the start pos\n";
	int nDir=dir.listDir(startPosition);
	bool found=false;
	for(int i = 0; i < nDir; i++){
		string fl=dir.getName(i);
		if(fl==incl+".h"){
			found=true;
      if(bVerbose) cout << "found " + incl + " in " + startPosition<<endl; 
			obj.addInclude(startPosition);
		}
		//if(fl.substr(fl.find_last_of('.'))==".cpp") objects.push_back(Object(dir.getPath(i)));
		else if(fl==incl+".cpp"||fl==incl+".c"){
			bool foundC=false;
			for(unsigned int j=0; j<objects.size()&&!found; j++){
				if(objects[j].filePath==dir.getPath(i)) foundC=true;
			}
			if(!foundC){
				objects.push_back(Object(dir.getPath(i)));
				computeDependencies(objects.back());
			}
		}
		else if(fl.find_last_of('.')==fl.npos&&!found&&fl!="Makefile"&&fl!="examples"){
			ret=searchFolder(incl,dir.getPath(i),obj);
			found=ret.length();
		}
	}
	return ret;
}

void compiler::compile(Object & obj)
{
	string file=obj.filePath;
	string base=obj.baseName;
	string root=obj.rootDir;
	bool isCpp=file.substr(file.find_last_of('.'))==".cpp";
	computeDependencies(obj);
	obj.addInclude(rootDir+"hardware/arduino/cores/arduino"); // change this to work with tinyX5;
	obj.addInclude(ofToDataPath(ROOT_DIR+"/include"));

	cmd.newCommand();
	if(isCpp) cmd.addArgument(toolDir+"avr-g++");
	else cmd.addArgument(toolDir+"avr-gcc");
	cmd.addArgument("-c -g -Os -w");
	if(isCpp) cmd.addArgument("-fno-exceptions");
	cmd.addArgument("-ffunction-sections -fdata-sections");
	cmd.addArgument("-DF_CPU="+freq+"L");
	cmd.addArgument("-DARDUINO=22");
	for(unsigned int i=0; i<obj.size(); i++){
		cmd.addArgument("-I"+obj[i]);
	}
	cmd.addArgument("-mmcu="+mcu);
	if(isCpp) cmd.addArgument(root+base+".cpp");
	else cmd.addArgument(root+base+".c");
	cmd.addArgument("-o " + appletDir+base+ ".o");

	percent=0;
	cmd.execute(bVerbose);

	/*
	path/to/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -DF_CPU=processor_speed
	-I(includes_for_cpp) -mmcu=processor_name path/to/target.cpp -o target.o
	*/
}

void compiler::compile(string filename)
{
	mainObj.setFilePath(filename);
	bSkipStep=false;
	mode=COMPILE;
	objects.clear();
	rep="Compiling...";
	appletDir=mainObj.filePath.substr(0,mainObj.filePath.find_last_of('/'))+"/";
	compile(mainObj);
	currentObj=0;
}

void compiler::compileDepends()
{
	mode=COMPILE_OBJS;
	if(objects.size()&&currentObj<objects.size()){
		ofxDirList dir;
    //cout << appletDir << endl;
		int nDir=dir.listDir(appletDir);
		rep="Compiling " + objects[currentObj].baseName + "...";
		bool exists=false;
		for(int i=0; i<nDir; i++){
			if(dir.getName(i)==objects[currentObj].baseName+".o") exists=true;
		}
		if(!exists) compile(objects[currentObj]);
		else bSkipStep=true;
	}
	else bSkipStep=true,currentObj=objects.size();
}

void compiler::linkLibraries()
{
	mode=LINK_LIB;
	if(currentObj<objects.size()){
		cmd.newCommand();
		cmd.addArgument(toolDir+"avr-ar");
		cmd.addArgument("rcs");
		cmd.addArgument(appletDir+"core.a");
		cmd.addArgument(appletDir+objects[currentObj].baseName+".o");

		cmd.execute(bVerbose);
	}
	else bSkipStep=true;
}
	
void compiler::link()
{
	mode=LINK;
	cmd.newCommand();
	cmd.addArgument(toolDir+"avr-gcc");
	cmd.addArgument("-Os -lm -Wl,--gc-sections");
	cmd.addArgument("-mmcu="+mcu);
	cmd.addArgument("-o "+appletDir+mainObj.baseName+".elf");
	cmd.addArgument(appletDir+mainObj.baseName+".o");
	for(unsigned int i=0; i<objects.size(); i++){
		cmd.addArgument(appletDir+objects[i].baseName+".o");
	}
	cmd.addArgument(appletDir+"core.a");

	cmd.execute(bVerbose);
}

void compiler::assemble()
{
	mode=ASSEMBLE;
	cmd.newCommand();
	cmd.addArgument(toolDir+"avr-objcopy");
	cmd.addArgument("-O ihex -R .eeprom");
	cmd.addArgument(appletDir+mainObj.baseName+".elf");
	cmd.addArgument(appletDir+mainObj.baseName+".hex");

	cmd.execute(bVerbose);

	/*

	To assemble .elf

		path/to/avr-gcc -Os -lm-Wl,--gc-sections -o path/to/target.elf path/to/target.o path/to/core.a

	To assemble .hex

		path/to/avr-objcopy -O ihex -R .eeprom path/to/target.elf path/to/target.hex

	*/
}

void compiler::upload(string port)
{
	mode=UPLOAD;
	cmd.newCommand();
	cmd.addArgument(toolDir+"avrdude");
	cmd.addArgument("-V -F -C");
	cmd.addArgument(toolDir+"../etc/avrdude.conf");
	cmd.addArgument("-p "+mcu);
	cmd.addArgument("-P "+port);
	cmd.addArgument("-c "+programmer);
	cmd.addArgument("-b "+baud);
	cmd.addArgument("-U flash:w:"+appletDir+mainObj.baseName+".hex");

	cmd.execute(bVerbose);

	/*
	To upload to the mcu

	path/to/avrdude -V -F -C path/to/avrdude.conf -p mcu_name -P port_name -c programmer_name
		-b transfer_rate -U flash:w:path/to/target.hex
	*/
}
	
void compiler::configure(string cfgFile)
{
	ifstream config(ofToDataPath(cfgFile).c_str());
	while (config.peek()!=EOF) {
		string nextLine;
		getline(config, nextLine);
		vector<string> token=ofSplitString(nextLine, "=\r\n");
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
			else if(token[0]=="EXCLUDE_PORT"){
				serChk->excludeDevice(token[1]);
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
				bVerbose=ofToInt(token[1]);
			}
		}
	}
	config.close();
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