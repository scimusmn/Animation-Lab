/*
 *  blockLoad.cpp
 *  robotBlocks
 *
 *  Created by Exhibits on 6/23/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "blockLoad.h"
#include "robotConfig.h"

/******************
 * config(string file) 
 *
 *  Description:: function to load ROOT_DIR information from 'file'
 *
 *
 *  Input_________
 *
 *    string file : filename of the config file which we wish to open
 *
 *  Output________
 *
 *    string : string containing the root directory as provided by the config file, else, "." or current directory
 *
 */


string config(string file){
  string ret;
	ifstream config(ofToDataPath(file).c_str());
	while (config.peek()!=EOF) {
		string nextLine;
		getline(config, nextLine);
		vector<string> token=ofSplitString(nextLine, "=");
		if(token.size()){
			if(!token[0].compare("ROOT_DIR")){
				ret = token[1];
			}
		}
		else {
			ret=".";
		}
		
	}
	config.close();
  return ret;
}

/******************
 * blockGroup(string dir) :: constructor for blockGroup
 *
 *  Description:: constructor for a new block group; calls the load function for a directory
 *
 *
 *  Input_________
 *
 *    string dir : path to the directory which we wish to index with the new blockGroup
 *
 *  Output________
 *
 *    New instance of blockGroup :
 *
 */

blockGroup::blockGroup(string dir)
{
  nLoaded=0;
  load(dir);
}

/******************
 * load(string dir) :: member of blockGroup
 *
 *  Description:: Searches through directory 'dir' to find "blocks.xml", "anim.xml", "icon.png" files and an optioinal "example.png" file
 *    if found, each file is loaded appropriately.
 *
 *  Input_________
 *
 *    string dir : directory to search for locating the relevant files
 *
 *  Output________
 *
 *    NONE : simply loads the files into the program.
 *
 */

void blockGroup::load(string dir)
{
  int nDir=0;
  nDir = DIR.listDir(dir);
  //you can now iterate through the files as you like
  for(int i = 0; i < nDir; i++){
      cout << DIR.getPath(i) << endl;
    vector<string> spl= ofSplitString(DIR.getPath(i), "/");
    if(spl.back()=="blocks.xml") blockXML.loadFile(DIR.getPath(i)), nLoaded++;
    else if(spl.back()=="anim.xml") animXML.loadFile(DIR.getPath(i)), nLoaded++;
	else if(spl.back()=="demos"){
		ofxDirList subdir;
		int num=subdir.listDir(DIR.getPath(i));
		for(int j=0; j<num; j++){
			size_t found=subdir.getPath(j).find(".xml");
			if(found!=string::npos){
				demos.push_back(ofXML());
				demos.back().loadFile(subdir.getPath(j));
			}
		}
	}
    else if(spl.back()=="icon_big.png") choice.setup(200,OF_HOR,DIR.getPath(i)), nLoaded++;
    else if(spl.back()=="example.png") example.loadImage(DIR.getPath(i)), bImgLoaded=true;
  }
  blockXML.setCurrentTag(";blocks");
  title=blockXML.getAttribute("title");
  subtitle=blockXML.getAttribute("subtitle");
}

/******************
 * load(string dir) :: member of rootGroup
 *
 *  Description:: Taking the root directory of the program as an argument, finds all folders which are not 'blocks', and creates a new
 *    blockGroup for each.
 *
 *  Input_________
 *
 *    string dir : string containing the name of the root directory
 *
 *  Output________
 *
 *    NONE : load block groups into rootGroup instance
 *
 */

void rootGroup::load(string dir)
{
  int nDir=0;
  nDir = DIR.listDir(dir);
  //you can now iterate through the files as you like
  for(int i = 0; i < nDir; i++){
    vector<string> spl= ofSplitString(DIR.getPath(i), "/");
    vector<string> spl2= ofSplitString(spl.back(), ".");
    if(spl2.size()==1){
		//if(atoi(spl2[0].substr(0,3).c_str()))
      bool found=0;
      for (unsigned int j=0; j<cfg().levelFolders.size(); j++) {
        if(cfg().levelFolders[j]==spl2[0]) found=true;
      }
      if(found){
		  if(cfg().verbose) cout << "Programming level " << set.size()+1 << " is " << spl2[0] << endl;
		  set.push_back(blockGroup(DIR.getPath(i)));
	  }
    }
  }
}

/******************
 * operator[](int i) :: member of rootGroup
 *
 *  Description:: accessor operator; returns the ith member of the blockGroup vector
 *
 *
 *  Input_________
 *
 *    int i :
 *
 *  Output________
 *
 *    blockGroup & :
 *
 */


blockGroup & rootGroup::operator[](int i)
{
  return set[i];
}

/*****************************************************************
 * operator()(int i) :: member of rootGroup
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    int i :
 *
 *  Output________
 *
 *    ofButton & :
 *
 */

ofButton & rootGroup::operator()(int i)
{
  return set[i].choice;
}

/*****************************************************************
 * getSelected() :: member of rootGroup
 *
 *  Description:: if there is a group selected, returns the selected member of 'set'; else returns 0;
 *
 *
 *  Input_________
 *
 *    NONE :
 *
 *  Output________
 *
 *    blockGroup * : returns the address of the currently chosen member of 'set'
 *
 */


blockGroup * rootGroup::getSelected()
{
  blockGroup * ret=0;
  for (unsigned int i=0; i<size(); i++) {
    if(set[i].choice.getAvailable()==false) ret=&set[i];
  }
  return ret;
}

/*****************************************************************
 * clickDown :: member of rootGroup
 *
 *  Description:: Handles the clickdown function for the ofButtons in each of the members of 'set'
 *
 *
 *  Input_________
 *
 *    int _x :  The x coordinate of the mouse click.
 *    int _y :  The y coordinate of the mouse click.
 *
 *  Output________
 *
 *    bool :    Returns whether or not a button was clicked.
 *
 */


bool rootGroup::clickDown(int _x, int _y)
{
  bool ret=0;
  for (unsigned int i=0; i<size(); i++) {
    if(set[i].choice.clickDown(_x,_y)){
      for (unsigned int j=0; j<size(); j++) {
        set[j].choice.setAvailable(true);
      }
      set[i].choice.setAvailable(false);
      ret=1;
    }
  }
  return ret;
}

/*****************************************************************
 * clickUp :: member of rootGroup
 *
 *  Description:: handles clickup for each of the buttons in the members of 'set'
 *
 *
 *  Input_________
 *
 *    NONE :
 *
 *  Output________
 *
 *    NONE :  Resets the clicked state of each of the buttons in 'set'
 *
 */


void rootGroup::clickUp()
{
  for (unsigned int i=0; i<size(); i++) {
    set[i].choice.clickUp();
  }
}

/******************
 * reset() :: member of rootGroup
 *
 *  Description:: Clears the availablility of the buttons.
 *
 *
 *  Input_________
 *
 *    NONE :
 *
 *  Output________
 *
 *    NONE :  All of the buttons are now available to be clicked.
 *
 */


void rootGroup::reset()
{
  for (unsigned int j=0; j<size(); j++) {
    set[j].choice.setAvailable(true);
  }
}