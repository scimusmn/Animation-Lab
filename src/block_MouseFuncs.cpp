/*
 *  newMouseFunctions.cpp
 *  robotBlocks
 *
 *  Created by Exhibits on 7/1/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */


#include "blocks.h"

/*****************************************************************
 * newClickInside(int _x, int _y) :: function of block
 *
 *  Description:: determines whether or not the block is a bracket block and if it has been clicked inside of.
 *
 *
 *  Input_________
 *
 *    int _x :  mouse click coords
 *     int _y :
 *
 *  Output________
 *
 *    bool :  returns true if we click on the interior of a bracket block
 *
 */


bool block::newClickInside(int _x, int _y)
{
  bool ret=false;
  if(type==BLK_BRACKET&&_x>x+interior.x&&_x<x+fullWidth()-interior.x&&_y>y+interior.y&&_y<y+interior.y+h) ret=true;
  return ret;
}

/*****************************************************************
 * clickDown(int _x, int _y) :: function of block
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    int _x :
 *     int _y :
 *
 *  Output________
 *
 *    bool :
 *
 */

bool block::clickDown(int _x, int _y)
{
  bool ret=false;
  if(over(_x,_y)&&!newClickInside(_x, _y)&&ddPassingClick(_x, _y)){
    ret=bGrabbed=true;
  }
  return ret;
}

/*****************************************************************
 * newClickDD(int _x, int _y, bool & ddopen) :: function of block
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    int _x :
 *     int _y :
 *     bool & ddopen :
 *
 *  Output________
 *
 *    bool :
 *
 */


bool block::newClickDD(int _x, int _y, bool & ddopen)
{
  bool ret=0;
  for (unsigned int j=0; j<ddGroup.size(); j++) {
		//-------- if we are over a any drop down with no drop downs open, or we are over the only open dd,
		if(((ddGroup[j].over(_x,_y)&&!ddopen)||ddGroup[j].open)){
			//-------- we check if we are over the dropdown
			if((ddopen=ddGroup[j].clickDown(_x,_y))){
				//-------- if we are, we set ddOpen to the dropdown number
				ddOpen=j+1;
			}
			else if(!ddopen) ddOpen=0; // if we are not over the dropdown, set ddOpen to false;
			ret=ddSelected=ret||ddGroup[j].selected; //update the selected bool.
		}
	}
  return ret;
}


//------------------Mouse motion/Dragging ----------------------

void block::motion(int _x, int _y)
{
	//-------- check if the mouse is inside an open dropDown; if so, change highlighting
	for (unsigned int i=0; i<ddGroup.size(); i++) {
		ddGroup[i].motion(_x,_y);
	}
}


//****************************** Click Functions ************************


bool block::ddPassingClick(int _x, int _y){
	//*********** this function checks whether or not a dd is open; if so, it returns false, and keeps the click
	bool ret=true;
	for (unsigned int i=0; i<ddGroup.size(); i++) {
		ret=ret&&ddGroup[i].passingClick(_x,_y);
	}
	return ret;
}

/*****************************************************************
 * newClickUp(int _x, int _y) :: function of block
 *
 *  Description:: mouse release function for the individual blocks;
 *    resets the bGrabbed member, and wipes selection vars of the dropdowns
 *    Also wipes all of the blocks below.
 *
 *  Input_________
 *
 *    int _x : x position of the mouse at click up
 *     int _y : y position   "        "        "
 *
 *  Output________
 *
 *    bool : false, always false. No point really
 *
 */

bool block::newClickUp(int _x, int _y)
{
  for (unsigned int j=0; j<ddGroup.size(); j++) {
    ddGroup[j].selected=false;
  }
  bGrabbed=ddSelected=false;
  for (unsigned int i=0; i<blocksIn.size(); i++) {
    blocksIn[i].newClickUp(_x,_y);
  }
  for (unsigned int i=0; i<blocksOn.size(); i++) {
    blocksOn[i].newClickUp(_x,_y);
  }
  ddSelected=false;
  return false;
}

//******************************** Individual functions **********************

/*****************************************************************
 * searchUnderBlock() 
 *
 *  Description:: function to find which vector and block the "held" block is under
 *
 *
 *  Input_________
 *
 *    dropBlock & foundBlock : a reference to the dropBlock container which holds the info for the block, if found.
 *    block & strt : a reference to the block under which we begin searching.
 *     block & drpd : reference to the held block
 *     blkVectType t : tells us if we are looking in the blockIn or blocksOn vector.
 *
 *  Output________
 *
 *    bool : true if we find a block which is above the held block in the specified vector, false otherwise.
 *
 */

bool searchUnderBlock(dropBlock & foundBlock,block & strt, block & drpd, blkVectType t)
{
  bool tmp=0;
  vector<block> * blck;
  
  //------- select which vector to examine
  if(t==OF_BLOCK_IN) blck=&strt.blocksIn;
  else blck=&strt.blocksOn;
  
  vector<block> & bV=*blck;
  
  //------- if we have not yet found a block under which the drpd block could be found
  if(!foundBlock.found()){
    
    //------- check directly under the strt block, if we're checking the blocksOn
    if(strt.type!=BLK_BRACKET||t==OF_BLOCK_ON){
      if (strt.beneath(drpd)) {
        tmp=true;
        foundBlock.set(strt,t,0);
      }
    }
    //------- if we're checking blocksIn, check immediately inside
    else {
      if (drpd.inBounds(strt.x+strt.interior.x,strt.y+strt.ttlSize.y/2, strt.w,((strt.blocksIn.size())?strt.ttlSize.y:strt.h-(strt.bottomBar)))) {
        tmp=true;
        //------- set the foundBlock to the 0 Position of the vector
        foundBlock.set(strt,t,0);
      }
    }
      
    //------- if we did not find the drpd block under the strt block immediately, iterate through the vector
    int spc=0;
    for (unsigned int i=0; i<bV.size()&&!tmp; i++) {
      block * nxt=0;
      spc=bV[i].ttlSize.y/2;
      //------- find the halfway point on the next block
      if(i<bV.size()-1) nxt=&(bV[i+1]);
      if(nxt) spc=nxt->ttlSize.y/2;
      if (bV[i].beneath(drpd)&&!bV[i].inside(drpd)) {
        tmp=true;
        foundBlock.set(strt,t,i+1);
      }
      else if(bV[i].inside(drpd)){
        tmp=searchUnderBlock(foundBlock,bV[i],drpd,OF_BLOCK_IN);
      }
    }
  }
  return tmp;
}

dropBlock underWhich(block & strt, block & drpd)
{
  dropBlock ret;
  bool tmp=0;
  if(strt.inside(drpd)){
    tmp=searchUnderBlock(ret,strt,drpd,OF_BLOCK_IN);
  }
  else if(!tmp&&strt.beneath(drpd,strt.newHeightOn())){
    tmp=searchUnderBlock(ret,strt,drpd,OF_BLOCK_ON);
  }
  
  return ret;
}
