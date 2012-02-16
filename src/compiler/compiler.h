/*
*   First attempt at a dependency compiler, a la Arduino. Used because the makefile
*	 is not a cross platform solution.
*
*/

#include "ofMain.h"
#include "ofExtended.h"
#include "ofxSystemCall.h"
#include "ofxDirList.h"

#pragma once

struct compile_config {
	string mcu,freq,programmer,rootDir,file,addonLib,baud;
	void load(string cfgFile);
};

class command {
private:
	ofSystemCall call;
	vector<string> args;
	bool bExecuting;
public:
	void newCommand();
	void addArgument(string arg);
	void execute();
	bool isExecuting();
	bool justExecuted();
	int linesOfOutput();
	string operator[](int i);
	string lastLine();
};

class compiler {
private:
	enum compMode { WAIT, DONE, COMPILER_ERROR, COMPILE, ASSEMBLE_ELF, ASSEMBLE_HEX, UPLOAD };
	compMode mode;
	command cmd;
	vector<string> includeFiles;
	vector<string> includePaths;
	compile_config conf;
	ofxDirList dir;
	string rootDir,file,fileRoot,addonLib,toolDir,port;
	string rep;
	float percent;
public:
	compiler();
	void compile(string filename,string prt);
	void update();
	void assemble();
	void upload(string port);
	void configure(string cfgFile);
	void checkForInclude(string & line);
	void findIncludeFolder(string & line);
	bool isCompiling();
	string report();
	float percentDone();
};