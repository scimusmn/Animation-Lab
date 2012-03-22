/*
*   First attempt at a dependency compiler, a la Arduino. Used because the makefile
*	 is not a cross platform solution.
*
*/

#include "ofMain.h"
#include "ofExtended.h"
#include "ofxSystemCall.h"
#include "ofxDirList.h"
#include "../serialChecker/serialCheck.h"

#pragma once

class command {
private:
	ofSystemCall call;
	vector<string> args;
	bool bExecuting;
public:
	void newCommand();
	void addArgument(string arg);
	void execute(bool echoToTerminal=false);
	bool isExecuting();
	bool justExecuted();
	int linesOfOutput();
	string operator[](int i);
	string lastLine();
	bool isThereOutput();
};

struct Object {
	string filePath;
	string rootDir;
	string baseName;
	Object();
	Object(string filename);
	void setFilePath(string path);
	vector<string> paths;
	void addInclude(string path);
	int size();
	string operator[](int i);
};

class compiler {
private:
	enum compMode { WAIT, DONE, COMPILER_ERROR, COMPILE, COMPILE_OBJS, LINK_LIB, LINK, ASSEMBLE, UPLOAD };
	compMode mode;
	command cmd;
	bool bSkipStep;
	string rootDir,addonLib,toolDir,port;
	string mcu,freq,programmer,baud;
	string rep;
	float percent;
	serialCheck * serChk;
	Object mainObj;
	vector<Object> objects;
	int currentObj;
	string appletDir;
	bool bVerbose;
public:
	compiler();
	void setup(serialCheck * srChk);
	void mainCompile(string fileName);
	void compile(Object & obj);

	void compile(string filename);
	void compileDepends();
	void linkLibraries();
	void link();
	void assemble();
	void upload(string port);

	void update();
	void configure(string cfgFile);
	void findIncludeFolder(string & line,Object & obj);
	string searchFolder(string & include, string startPosition, Object & obj);
	void computeDependencies(Object & obj);
	bool isCompiling();
	string report();
	float percentDone();
};