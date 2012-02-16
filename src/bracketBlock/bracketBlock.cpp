/*
 *  bracketBlock.cpp
 *  arenaBot
 *
 *  Created by Exhibits on 11/7/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "bracketBlock.h"

bracketBlock::bracketBlock(ofTag &t,ofColor k):block(t,k)
{
  type=BLK_BRACKET;
}