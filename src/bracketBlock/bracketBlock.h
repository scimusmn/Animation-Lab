/*
 *  bracketBlock.h
 *  arenaBot
 *
 *  Created by Exhibits on 11/7/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#pragma once

#include "../blocks.h"

#define is_bracket(x) ((x.type==BLK_BRACKET)?static_cast<bracketBlock>(x):x)

#define as_bracket(x) static_cast<bracketBlock>(x)

class bracketBlock : public block {
protected:
  
public:
  bracketBlock(ofTag &t,ofColor k);
};
