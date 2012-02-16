#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofExtended.h"
#include "sideBar/sideBar.h"

#include "controlBar/controlBar.h"

/*****************************************************************
 * class testApp : public ofBaseApp
 *
 *  Description:: 
 *
 *
 *  Vars:
 *
 *    int animStep:: _ _ _ _ _ _ _ _ the current animation step, if playing.
 *    ofXML animXML::_ _ _ _ _ _ _ _ the xmlParser to hold the data for the current animation
 *    bGroup blocks::_ _ _ _ _ _ _ _ the block group, containing the base block and all the blocks which have been drag onto the stage
 *    sbGroup * sidebar::_ _ _ _ _ _ the sidebar bars and blocks
 *    ofButton upBut:: _ _ _ _ _ _ _ the upload button, possibly should be migrated to the 'bGroup'
 *    ofButton clearBut::_ _ _ _ _ _ button to clear the workspace blocks
 *    ofButton redoBut:: _ _ _ _ _ _ button to redo any block action previously undone
 *    ofButton undoBut:: _ _ _ _ _ _ button to undo any block action
 *    ofButton skipBut:: _ _ _ _ _ _ button that is displayed during an animation to skip the animation
 *    ofTimer timeOut::_ _ _ _ _ _ _ exhibit timer
 *    ofFont titleFont:: _ _ _ _ _ _ font used for large labels
 *    ofFont label:: _ _ _ _ _ _ _ _ font used to write on the base block. Should be migrated.
 *    ofSystemCall systemCall::_ _ _ system call thread, to run the upload script
 *    ofProgressSpinner spinner::_ _ spinner to indicate when the program is uploading
 *    ofVMouse anim::_ _ _ _ _ _ _ _ virtual mouse class used to demonstrate
 *    ofImage pointer::_ _ _ _ _ _ _ pointer used by the mouse and by the virtualMouse
 *    ofButton demo::_ _ _ _ _ _ _ _ button to start the demo
 *    rootGroup sets:: _ _ _ _ _ _ _ storage container for the block data
 */




class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
  
  void drawBase_and_Demo(int yPosition);
  void drawSubtitleBar(int yPos, int hgt, int tabHgt);
  void drawTitleBar(int & height);
  void drawMenuBar(int & yPos, int & hgt);
  void drawSidebar(int sideWidth, int menuBarH);
  void drawAttract();
  

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
	void upload();
	
	bGroup blocks;
	sbGroup sidebar;
  
  titleBar topTitle;
	
  rootGroup sets;
  
  controlBar controls;
  
  /* to delete */
  
  
  /////////////////
  
  ofImage mapps;
  
};

#endif
