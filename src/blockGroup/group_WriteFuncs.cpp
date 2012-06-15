/*
 *  groupWriteFuncs.cpp
 *  arenaBot
 *
 *  Created by Exhibits on 11/1/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */


#include "blockGroup.h"
#include "../robotConfig.h"

extern int CURRENT_ROBOT_NUMBER;

//------------------ bGroup Writeout Functions ----------------

void bGroup::resetUsed(block & t)
{
	//-------- resets the log of what blocks have been used in the last print stage
	used[""]=false;
	used[t.title]=false;
	for (unsigned int i=0; i<t.numInside(); i++) {
		resetUsed(t.blocksIn[i]);
	}
	for (unsigned int i=0; i<t.size(); i++) {
		resetUsed(t.blocksOn[i]);
	}
}

/*****************************************************************
 * resetUsed() :: function of bGroup
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    NONE :
 *
 *  Output________
 *
 *    NONE :
 *
 */

void bGroup::resetUsed()
{
	resetUsed(base);
	for (unsigned int i=0; i<blocks.size(); i++) {
		resetUsed(blocks[i]);
	}
}

/*****************************************************************
 * writeFile(string filename) :: function of bGroup
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    string filename :
 *
 *  Output________
 *
 *    NONE :
 *
 */

void bGroup::writeFile(string filename)
{
  ofstream out(ofToDataPath(filename).c_str());
	writeFile(&out);
	out.close();
}

/*****************************************************************
 * writeFile(ofstream *k) :: function of bGroup
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    ofstream *k :
 *
 *  Output________
 *
 *    NONE :
 *
 */

void bGroup::writeFile(ofstream *k){
	//blocks[lastBlock].printOut(k);
	if(blocks.size()||1){
		//-------- Updated to allow for greater flexability (ie we can change basic behavior, without recompile
		
		//-------- open the path to the wrapper file, which contains general program structure.
		string buffer;
		ifstream f(ofToDataPath(cfg().wrapperFile).c_str());
		while ((f).peek()!=EOF) {
			//-------- read the next line into the buffer and init the position keepers and the tab count
			getline((f),buffer);
			int strtPos=0,endPos=0;
			int tabs=0;
			for (unsigned int i=0; i<buffer.length(); i++) {
				//-------- if we find a tab, increment; if the next char is not a '$' print out a tab.
				if(buffer[i]=='\t'){
					tabs++;
					if(buffer[i+1]!='$') *k << '\t';
				}
				//-------- if we find a '$'
				else if(buffer[i]=='$'){
					//-------- find the next word after the $
					strtPos=endPos=++i;
					while (buffer[endPos]!='.'&&buffer[endPos]!=','&&buffer[endPos]) {
						endPos++;
					}
					i=endPos;
					string temp(buffer,strtPos,endPos-strtPos);
					//-------- if it was "blocks"
					if(!temp.compare("blocks")){
						//-------- find the word after the '.'
						strtPos=endPos=++i;
						while (buffer[endPos]!='('&&buffer[endPos]) {
							endPos++;
						}
						string temp2(buffer,strtPos,endPos-strtPos);
						
						//-------- reset the used block map
						resetUsed();
						
						//-------- print out the appropriate routines, according to the name that followed the '.'
						if(!temp2.compare("global")){
							//blocks[lastBlock].printGlobal(k,0,&used);
							base.printData("global(){",k,0,&used);
						}
						else if(!temp2.compare("setup")){
							//blocks[lastBlock].printSetup(k,tabs,&used);
							base.printData("setup(){",k,tabs,&used);
						}
            else if(!temp2.compare("start")){
							//blocks[lastBlock].printSetup(k,tabs,&used);
							base.printData("start(){",k,tabs,&used);
						}
						else if(!temp2.compare("loop")){
							base.printData("loop(){",k,tabs,&used,false);
						}
            else if(!temp2.compare("end")){
							//blocks[lastBlock].printSetup(k,tabs,&used);
							base.printData("end(){",k,tabs,&used);
						}
						i=endPos+1;
					}
          else if(temp=="deviceNumber"){
            *k << CURRENT_ROBOT_NUMBER;
            i=endPos;
          }
				}
				//-------- if we didn't find either of those things, write the i^th character to k. 
				else {
					*k << buffer[i];
				}
			}
			//-------- end each line with a newline.
			*k << endl;
		}
	}
}

//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_
//_-_-_-_-_//_-_-_-_-_ xml save  //_-_-_-_-_//_-_-_-_-_

void bGroup::saveXML(string filename){
  ofXML save;
  save.newCurrentTag("base");
  ofTag & top=save.getCurrentTag();
  top.addAttribute("robot_number", ofToString(CURRENT_ROBOT_NUMBER));
  for (unsigned int i=0; i<base.blocksOn.size(); i++) {
    top.addNode(base.blocksOn[i].saveTag());
  }
  save.writeFile(filename);
  bChanged=false;
}

bool bGroup::changedSinceSave()
{
  return bChanged;
}

void bGroup::loadFile(string filename)
{
  ofXML xml;
  clear();
  xml.loadFile(filename);
  xml.setCurrentTag(";base");
	ofTag & tag=xml.getCurrentTag();
	for (unsigned int i=0; i<tag.size(); i++) {
    cout <<tag[i].getLabel()<< endl;
		if (tag[i].getLabel()=="block") {
			base.blocksOn.push_back(block());
			base.blocksOn.back().setup(tag[i],base.color);
		}
	}
}
