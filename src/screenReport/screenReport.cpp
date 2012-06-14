#include "screenReport.h"

static screenReport defaultRep;

screenReport & report()
{
	return defaultRep;
}

screenReport::screenReport()
{
	
}

void screenReport::setup()
{
	rep.loadFont("fonts/DinC.ttf");
	rep.setSize(60);
	rep.setMode(OF_FONT_TOP);
	rep.setMode(OF_FONT_CENTER);
	timer.set(3);
	timer.pause();
}

void screenReport::post(string report, double time)
{
	repString=report;
	timer.set(time);
	timer.run();
}

void screenReport::draw()
{
	if(timer.running()){
		ofSetColor(0, 0, 0,128);
		ofRect(0, 0, ofGetWidth(), ofGetHeight());
		vector<string> spl=ofSplitString(repString,"\n");
		float wid=0;
		for(unsigned int i=0; i<spl.size(); i++){
			wid=max(rep.stringWidth(spl[i]),wid);
		}
		float hgt=spl.size()*(rep.stringHeight("Kjgh")+20)-20;
		drawStyledBox((ofGetWidth()-wid)/2-50, (ofGetHeight()-hgt)/2-50, wid+100, hgt+100);
		ofSetColor(255,255,255);
		//subtitle.drawString("Program changes not uploaded", ofGetWidth()/2, ofGetHeight()/3);
		for(unsigned int i=0; i<spl.size(); i++){
			rep.drawString(spl[i],ofGetWidth()/2,(ofGetHeight()-hgt)/2+(rep.stringHeight("Kjgh")+20)*i);
		}
		//subtitle.drawString("Reconnect robot to upload.", ofGetWidth()/2, ofGetHeight()/2.);
	}
}

bool screenReport::isReporting()
{
	return timer.running();
}