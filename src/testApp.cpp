 #include "testApp.h"
#include "robotConfig.h"


string curDir="basic";

extern int pixPerInch;

extern ofColor black,white,yellow,blue,orange, gray;

extern bool tagAvailable;

bool bCopy=false;

//--------------------------------------------------------------
void testApp::setup(){
	cfg().setup();

	//--------- Load font for drawing on screen

	if(cfg().titleBarSize) topTitle.loadFont("fonts/DinC.ttf", cfg().titleBarSize);
	else topTitle.h=0;
  
	ofHideCursor();
  
  //mapps.loadImage("maps/map_2.jpg");
  
  controls.setup(&blocks, &sidebar);

  //--------- Initialize the valid working space for the blocks
	blocks.setup(sidebar.area.width,controls.h+topTitle.h,ofGetWidth()-sidebar.area.width,ofGetHeight()-(controls.h+topTitle.h));
	blocks.setScrolling(cfg().scroll);
}

//--------------------------------------------------------------
void testApp::update(){

	//blocks.update();
  controls.update();
}

//--------------------------------------------------------------
void testApp::draw(){
  //--------- Draw background with slightly yellow grid over it.
	if(!cfg().defaultColor)
		ofBackground(cfg().backgroundColor.r,cfg().backgroundColor.g, cfg().backgroundColor.b);
	else ofBackground(gray.r,gray.g, gray.b);
  
  
  if(cfg().defaultColor) ofSetColor(black);
  else ofSetColor(cfg().lineColor.opacity(.1));
  if(cfg().defaultColor) drawHatching(0, 0, ofGetWidth(), ofGetHeight(), 15,1);
  else drawHatching(0, 0, ofGetWidth(), ofGetHeight(), 10,1);
  
  //******************** Menu Bar **************************
  
  blocks.draw(sidebar.area.width,controls.h+topTitle.h,ofGetWidth()-sidebar.area.width,ofGetHeight()-(controls.h+topTitle.h));
  
  sidebar.draw(0,controls.h+topTitle.h);
  
  controls.draw(0, topTitle.h);

  if(cfg().showTitle) topTitle.draw(cfg().robotTitle,0,0);
  
  //********************** Draw the blocks which are being held by the mouse ********
  
  blocks.drawForeground();
	
	//************************ if we're uploading, draw a fancy "uploading..." on screen
	
  
  
  
  controls.drawForeground();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
  if(key=='c') bCopy=true;
  if(key=='t') tagAvailable=!tagAvailable;
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){
  if(key=='c') bCopy=false;
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	//--------- if the mouse is moving and not clicked, inform the blocks
	blocks.motion(x, y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
	//--------- if we are dragging the mouse, tell blocks so it can update the positions of any of the held blocks
  if(!controls.mouseLockout(button))
     blocks.drag(x, y);
}

//***************** Mouse Click down function ***************
void testApp::mousePressed(int x, int y, int button){
  
  controls.clickDown(x, y,button);
  
  //--------- Check the blocks in the sidebar, to see if they have been clicked
  if(!controls.mouseLockout(button))
    sidebar.clickDown(x, y);
  
  //--------- Check the blocks in the comp area
  if(!controls.mouseLockout(button))
    blocks.clickDown(x, y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
  controls.clickUp();
	if(!controls.mouseLockout(button)){
    //--------- do a bunch of clickups
    blocks.newClickUp(x, y);
  }
  controls.setAvailableButtons();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	controls.resize();
	blocks.resize(sidebar.area.width,controls.h+topTitle.h,ofGetWidth()-sidebar.area.width,ofGetHeight()-(controls.h+topTitle.h));
}

