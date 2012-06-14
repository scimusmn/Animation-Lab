#pragma once

#include "ofMain.h"
#include "ofExtended.h"
#include "../../../dallasEng/dallasEng.h"

class screenReport {
protected:
	ofTimer timer;
	ofFont rep;
	string repString;
	bool bSetup;
public:
	screenReport();
	void setup();
	void post(string report, double time);
	void draw();
	bool isReporting();
};

screenReport & report();