/*
 *  demoAnim.cpp
 *  arenaBot
 *
 *  Created by Exhibits on 11/7/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "demoAnim.h"
#include "../robotConfig.h"
#include "ofMain.h"

extern ofColor black, yellow, white, gray;

void demoAnim::setup(bGroup * bG, sbGroup * sbG)
{
  blocks=bG;
  sideBar=sbG;

  bPrompt=false;
  
  anim.setup(this);
  animStep=0;
  pointer.loadImage("images/pointer.png");
  
  yes.setup("Yes", 40);
  no.setup("No",40);
  
  yes.w=no.w=max(yes.w*1.5,no.w*1.5);
  
  header.loadFont("fonts/DinC.ttf");
  header.setSize(70);
  header.setMode(OF_FONT_TOP);
  header.setMode(OF_FONT_CENTER);
}

void demoAnim::changeBlockLevel(blockGroup & bg)
{
	set=&bg;
	demoButtons.clear();
	if(bg.demos.size()==1){
		changeAnimXML(bg.demos[0]);
		no.setTitle("No");
	}
	else if(bg.demos.size()>1){
		no.setTitle("No demo");
		for(unsigned int i=0; i<bg.demos.size(); i++){
			bg.demos[i].setCurrentTag(";animation");
			demoButtons.push_back(dallasButton());
			demoButtons.back().setup(bg.demos[i].getCurrentTag().getAttribute("title"),40);
		}
	}
}

void demoAnim::changeAnimXML(ofXML & newXML)
{
  animXML=newXML;
}

ofTag moveTag(string speed,string obj="", string x="", string y=""){
  ofTag ret("step");
  ret.addAttribute("type", "move");
  ret.addAttribute("speed",speed);
  ret.addNode("pos");
  ofTag & nd=ret.getNode("pos");
  nd.addAttribute("type","obj");
  nd.addAttribute("where",obj);
  nd.addAttribute("x",x);
  nd.addAttribute("y",y);
  return ret;
}

ofTag clickDownTag(){
  ofTag ret("step");
  ret.addAttribute("type","clickDown");
  ret.addAttribute("time", ".25");
  return ret;
}

ofTag clickUpTag(){
  ofTag ret("step");
  ret.addAttribute("type","clickUp");
  ret.addAttribute("time", ".25");
  return ret;
}

void demoAnim::handleAnimStep(ofTag tag)
{
  //------- set the default event type to "end"
  vMouseType vType=OF_VMOUSE_END;
  //------- variable declarations
  ofInterObj * object=0;
  double duration=0;
  double speed=0;
  int xint=0, yint=0;
  
  string type=tag.getAttribute("type");
  if(type.length()){
    if(type=="move") vType=OF_VMOUSE_MOVE_TO;
    else if(type=="clickDown") vType=OF_VMOUSE_CLICK_DOWN,xint=anim.x,yint=anim.y;
    else if(type=="clickUp") vType=OF_VMOUSE_CLICK_UP,xint=anim.x,yint=anim.y;
    else if(type=="select"){
      
    }
  }
  
  //------- if we have a time attribute, set the duration var to that attribute
  string time=tag.getAttribute("time");
  if(time.length()){
    duration=ofToFloat(time);
  }
  //-------otherwise, we need to look for the speed at which we want to move, to be used later to determine time
  else {
    string spd=tag.getAttribute("speed");
    if(spd.length()){
      speed=ofToFloat(spd);
    }
  }
  //------- if there is a pos tag inside the current tag, push into it
  if(tag.getNumTags("pos")){
    ofTag & tg=tag.getNode("pos");
    //------- and look at the type of position tag it is 
    string typ=tg.getAttribute("type");
    if(typ=="obj") //-- if it is an object tag, search for the specified object using the searchForObject function
      object = searchForObject(tg,xint,yint);
    else if(typ=="coord"){
      //-------if it is a coordinate, set xint and yint to the x and y attributes.
      xint=ofToInt(tg.getAttribute("x"));
      yint=ofToInt(tg.getAttribute("y"));
    }
    //-------if it is a mouse tag, set the xint and yint to the mouse coordinates
    else if(typ=="mouse") xint=ofGetAppPtr()->mouseX,yint=ofGetAppPtr()->mouseY;
    
    if(!duration){ //-- if we haven't yet set the duration, use the speed to determine duration
      int xtmp=((object)?object->x+xint:xint);
      int ytmp=((object)?object->y+yint:yint);
      double dist= sqrt(double((xtmp-anim.x)*(xtmp-anim.x)+(ytmp-anim.y)*(ytmp-anim.y)));
      duration=dist/speed;
    }
    
    //-- if we are following an object, the next event is called, referencing that object
    if(object) anim.nextEvent(vType,(*object),xint, yint,duration);
    else anim.nextEvent(vType,xint, yint,duration);  //-- otherwise, it is called using only x and y
  }
  else { //-- if there is not a position tag inside, the next event takes place where the vMouse is currently located
    anim.nextEvent(vType,xint, yint,duration);
  }
}

/**
 *  animationStepRequested(ofXML & animXML)- handles next step of an animation 
 *
 *  this function takes input from an xml file and, based on the animStep
 * pushes the approriate step to the anim class.
 *
 *  Input_________
 *    ofXML & animXML-- parsed xml file to be used in determining the next step;
 *
 *  Return________
 *    NONE: function is a void.
 */

void demoAnim::animationStepRequested()
{
  //------- try to set the current tag for the current animation XML file
  if(animXML.setCurrentTag(";animation")){
    //------- if the current animation step is less than the total number of steps
    if(animStep<animXML.getNumTag("step")){
      //------- push into the tag and fetch the event Type
      animXML.pushTag("step",animStep);
	  if(animXML.getCurrentTag().getAttribute("type")=="select"){
		  ofTag& tag=animXML.getCurrentTag();
		  animXML.setCurrentTag(";animation");
		  string whr=tag.getAttribute("where");
		  string which=tag.getAttribute("which");
		  animXML.getCurrentTag().removeNode("step",animStep);
		  animXML.addTag(moveTag("2000",whr,"w/2","h/2"),animStep);
		  animXML.addTag(clickDownTag(),animStep+1);
		  animXML.addTag(clickUpTag(),animStep+2);
		  animXML.addTag(moveTag("2000",whr,"w/2","h*"+which+".5"),animStep+3);
		  animXML.addTag(clickDownTag(),animStep+4);
		  animXML.addTag(clickUpTag(),animStep+5);
		  animXML.pushTag("step",animStep);
		  animXML.writeFile("test.xml");
	  }
      handleAnimStep(animXML.getCurrentTag());
    }
    //-- if the animStep is greater than the current number of steps in the xml, end the animation
    else anim.nextEvent(OF_VMOUSE_END,ofGetAppPtr()->mouseX,ofGetAppPtr()->mouseY,0);
  }
  animStep++;
}

/*****************************************************************
 * searchForObject(ofTag & tag, int & _x, int & _y) :: function of demoAnim
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    ofTag & tag :
 *     int & _x :
 *     int & _y :
 *
 *  Output________
 *
 *    ofInterObj * :
 *
 */


ofInterObj * demoAnim::searchForObject(ofTag & tag, int & _x, int & _y)
{
  ofInterObj * ret=0;
  string where=tag.getAttribute("where");
  string xTemp=tag.getAttribute("x");
  string yTemp=tag.getAttribute("y");
  vector<string> whSplit = ofSplitString(where, "[].");
  if(whSplit[0]=="sidebar"){
    if(whSplit.size()==2) ret=((*sideBar)[ofToInt(whSplit[1])]);
    if(whSplit.size()==3) ret=&((*(*sideBar)[ofToInt(whSplit[1])])[ofToInt(whSplit[2])]);
  }
  if(whSplit[0]=="openBar"){
	  if(whSplit.size()==2&&sideBar->openBar()){
		  if(whSplit[1]=="drop") ret=&((as_dynamic(sideBar->openBar()))->select);
		  else ret=&((*(sideBar->openBar()))[ofToInt(whSplit[1])]);
	  }
  }
  else if(whSplit[0]=="sidebarButton"){
    if(whSplit.size()==2) ret=&((*sideBar)[ofToInt(whSplit[1])])->button;
  }
  else if(whSplit[0]=="base"){
    if(whSplit.size()==1) ret=&(blocks->base);
    else if(whSplit.size()>=2&&whSplit[1]=="last") ret=searchBlock(whSplit,blocks->base.blocksOn.back(),2);
    else if(whSplit.size()>=2) ret=searchBlock(whSplit,blocks->base.blocksOn[ofToInt(whSplit[1])],2);
  }
  else if(whSplit[0]=="upload"){
    ret=&blocks->base.uploadBut;
    //TODO: update the base block to include the upload button.
  }
  else if(whSplit[0]=="test"){
    ret=&blocks->base.testBut;
  }
  vector<string> xSpl = ofSplitString(xTemp, "+-/*");
  if(xSpl.size()==1) _x=ofToInt(xSpl[0]);
  else if(xSpl.size()==2) _x=operaterByChar(ret->w,xTemp[xSpl[0].length()],ofToFloat(xSpl[1]));
  vector<string> ySpl = ofSplitString(yTemp, "+-/*");
  if(ySpl.size()==1) _y=ofToInt(ySpl[0]);
  else if(ySpl.size()==2) _y=operaterByChar(ret->h,yTemp[ySpl[0].length()],ofToFloat(ySpl[1]));
  
  return ret;
}

ofInterObj * demoAnim::searchBlock(vector<string> spl, block & currentBlock, int offset)
{
  ofInterObj * ret=0;
  if(offset>=spl.size()) ret=&currentBlock;
  else {
    string which=spl[offset];
    int num=((offset>=spl.size()-1)?ofToInt(spl[offset+1]):0);
    if(which=="inside") ret=searchBlock(spl,currentBlock.blocksIn[ofToInt(spl[offset+1])],offset+2);
    else if(which=="drop") ret=&(currentBlock.ddGroup[ofToInt(spl[offset+1])]);
  }
  return ret;
}

void demoAnim::update()
{
  if(anim.isPlaying())
    anim.updateNextEvent();
}

void demoAnim::drawCursor()
{
  ofSetColor(255, 255, 255);
  if(anim.isPlaying()){
    if(anim.isClicked()) ofSetColor(128, 128, 128);	
    pointer.draw(anim.x-10, anim.y, pointer.width*2,pointer.height*2);
    ofSetColor(128,128,128,128);
  }
  pointer.draw(ofGetAppPtr()->mouseX-10, ofGetAppPtr()->mouseY, pointer.width*2,pointer.height*2);
}

void demoAnim::startPrompt()
{
  bPrompt=true;
}

void demoAnim::clearPrompt()
{
  bPrompt=false;
}

bool demoAnim::isPrompting()
{
  return bPrompt;
}

void demoAnim::drawForeground()
{
  if(bPrompt){
    ofSetColor(black.opacity(200));
    ofRect(0,0,ofGetWidth(),ofGetHeight());
	if(!demoButtons.size()){
		string head="View Demo?";
		int w=max(double(header.stringWidth(head)),yes.w*2+50);
		int h=header.stringHeight(head)+50+no.h;
		ofRectangle r((ofGetWidth()-w)/2-50, (ofGetHeight()-h)/2-50, w+100, h+100);
		drawStyledBox(r.x,r.y,r.width,r.height);
		header.drawString(head, r.x+r.width/2, r.y+50);
		yes.draw(r.x+r.width/2-yes.w-25, r.y+r.height-50-yes.h);
		no.draw(r.x+r.width/2+25, r.y+r.height-50-no.h);
	}
	else{
		string head="Select a demo.";
		double w=max(double(header.stringWidth(head)),no.w+50);
		int h=header.stringHeight(head)+50+no.h;
		for(unsigned int i=0; i<demoButtons.size(); i++){
			h+=demoButtons[i].h+25;
				w=max(w,demoButtons[i].w+50);
		}
		ofRectangle r((ofGetWidth()-w)/2-25, (ofGetHeight()-h)/2-25, w+50, h+50);
		drawStyledBox(r.x,r.y,r.width,r.height);
		ofSetColor(white);
		header.drawString(head, r.x+r.width/2, r.y+50);
		//yes.draw(r.x+r.width/2-yes.w-25, r.y+r.height-50-yes.h);
		int totH=25;
		for(unsigned int i=0; i<demoButtons.size(); i++){
			demoButtons[i].draw(r.x+(r.width-demoButtons[i].w)/2,r.y+header.stringHeight(head)+50+totH);
			totH+=demoButtons[i].h+25;
		}
		no.draw(r.x+(r.width-no.w)/2, r.y+r.height-25-no.h);
	}

  }
}

bool demoAnim::clickDown(int x, int y)
{
	bool ret=false;
	if(bPrompt){
		if(demoButtons.size()){
			for(unsigned int i=0; i<demoButtons.size()&&!ret; i++){
				if(demoButtons[i].clickDown(x,y)){
					if(set) changeAnimXML(set->demos[i]);
					play();
					bPrompt=false, ret=true;
				}
			}
			if(!ret&&no.clickDown(x,y)) bPrompt=false,ret=true;
		}
		else {
			if(yes.clickDown(x, y)){
				if(set) changeAnimXML(set->animXML);
				play();
				bPrompt=false,ret=true;
			}
			if(no.clickDown(x, y)) bPrompt=false,ret=true;
		}
	}
	return ret;
}

bool demoAnim::clickUp()
{
  yes.clickUp();
  no.clickUp();
  return false;
}


void demoAnim::play()
{
  blocks->clear();
  animStep=0;
  anim.play();
}

bool demoAnim::isPlaying()
{
  return anim.isPlaying();
}

void demoAnim::stop()
{
  anim.stop();
}

