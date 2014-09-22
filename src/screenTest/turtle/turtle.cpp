/*
 *  turtle.cpp
 *  robotBlocks
 *
 *  Created by Exhibits on 7/19/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "turtle.h"

extern int pixPerInch;

extern ofColor black, white, gray, yellow, blue, red, orange;

ofTurtle::ofTurtle()
{
  start.x=start.y=0;
  pos.x=pos.y=0;
  w=h=0;
}

void ofTurtle::setup(int _x, int _y, int _w, int _h)
{
  numLineStored=pixPerInch*16;
  start.x=pos.x=_x,start.y=pos.y=_y,w=_w,h=_h;
  whlHgt=h/2;
  whlWid=w/8;
  bearing=ofVector(0,-1);
  bCompleted=bCrashed=false;
}

void ofTurtle::setup(string filename)
{
  map.loadImage(filename);
  int ppi=pixPerInch=map.width/48;
  ofPoint wheel(.25*ppi,(2+3/8)*ppi);
  ofPoint rWheel(1.125*ppi,1.875*ppi);
  setup(2*(map.width/12.),map.height-3*(map.height/12.), pixPerInch*5,7.25*pixPerInch);
  ofRectangle body(-3.25*ppi/2,-wheel.y/2,ppi*3.25,ppi*4.5);
  bdy.push_back(ofVector(body.x,body.y));
  bdy.push_back(ofVector(body.x+body.width,body.y));
  bdy.push_back(ofVector(body.x,body.y+body.height));
  bdy.push_back(ofVector(body.x+body.width,body.y+body.height));
  //bdy.push_back(ofVector(body.x+(body.width-rWheel.x)/2,body.y+body.height+rWheel.y));
  //bdy.push_back(ofVector(body.x+(body.width+rWheel.x)/2,body.y+body.height+rWheel.y));
  //ofRect(body.x+(body.width-ppi*1.375)/2+.125*ppi, body.y+body.height+.5*ppi, .875*ppi, 1.5*ppi);
}

void ofTurtle::reset()
{
  clear();
  pos=start;
  bearing=ofVector(0,-1);
  bCompleted=bCrashed=false;
}

bool ofTurtle::checkPoints()
{
  bool ret=0;
  unsigned char * k=map.getPixels();
  if(!bCrashed){
    for (unsigned int i=0; i<bdy.size(); i++) {
      ofVector t=bdy[i];
      ofPoint ps=pos+t.rotate(360-bearing.absoluteAngle());
      if(ps.x>0&&ps.x<map.width&&ps.y>0&&ps.y<map.height){
        if(k[int(ps.y)*map.width*3+int(ps.x)*3+1]>200){
          ret=bCrashed=true;
        }
      }
      else ret=bCrashed=true;
    }
    if(!ret&&k[int(pos.y)*map.width*3+int(pos.x)*3+2]>200){
      //cout << "Reached destination" << endl;
      bCompleted=true;
    }
  }
  return ret;
}

void ofTurtle::move(int pixels)
{
  pos=pos+bearing.unit()*pixels;
  if(!bTurnSinceRecord&&lines.size()>1) lines.pop_front();
  else bTurnSinceRecord=false;
  lines.push_front(pos+bearing.unit()*pixels);
  if(lines.size()>numLineStored) lines.pop_back();
}

void ofTurtle::turn(int degrees)
{
  bTurnSinceRecord=true;
  bearing.rotate(degrees);
}

bool ofTurtle::sensorIsClear(ofPoint strtPnt,int pixels, ofImage & walls, int direction)
{
  bool ret=true;
  unsigned char * k=walls.getPixels();
  for (int i=0; i<4; i++) {
    ofPoint ps=strtPnt+bearing.unit().rotate(direction)*(i+1)*pixels/4;
    int wid=walls.width;
    int hgt=walls.height;
    if(ps.x>0&&ps.x<wid&&ps.y>0&&ps.y<hgt){
      //ret=true;
      if(k[int(ps.y)*wid*3+int(ps.x)*3+1]<200){
        ret&=true;
      }
      else ret=false;
    }
  }
  return ret;
}

bool ofTurtle::frontIsClear(int pixels, ofImage & walls)
{
  frontCheckDist=pixels;
  return sensorIsClear(pos, pixels+(2+3/8)*pixPerInch/2, walls);
}

bool ofTurtle::leftIsClear(int pixels, ofImage & walls)
{
  leftCheckDist=pixels;
  ofPoint ps=pos+bearing.ortho().unit()*w/2-bearing.unit()*h/2;
  return sensorIsClear(ps, pixels, walls,270);
}

bool ofTurtle::frontIsClear(int distance)
{
  return frontIsClear(distance,map);
}

bool ofTurtle::leftIsClear(int distance)
{
  return leftIsClear(distance,map);
}

ofPoint ofTurtle::pointAlongBearing(int pix)
{
   return pos+bearing.unit()*pix;
}

void ofTurtle::draw(int _x, int _y)
{
  int ppi=pixPerInch;
  ofPoint wheel(.25*ppi,(2+3/8)*ppi);
  ofPoint rWheel(1.125*ppi,1.875*ppi);
  ofRectangle body(-3.25*ppi/2,-wheel.y/2,ppi*3.25,ppi*4.5);
  //int body=w-whlWid*2;
  //int leng=h;
  ofEnableSmoothing();
  for (unsigned int i=0; i<lines.size()-1&&lines.size()>1; i++) {
    ofSetLineWidth(2);
    ofLine(lines[i].x, lines[i].y, lines[i+1].x, lines[i+1].y);
  }
  ofDisableSmoothing();
  ofPushMatrix();
  ofTranslate(pos.x, pos.y, 0);
  ofRotate(360-bearing.absoluteAngle());
  
  ofEnableSmoothing();
  ofSetColor(black.opacity(128));
  ofRect(body.x,body.y,body.width,body.height);
  ofSetColor(white);
  ofNoFill();
  ofRect(body.x,body.y,body.width,body.height);
  ofFill();
  ofSetColor(black);
  ofRect(body.x-ppi*3/8, body.y, wheel.x, wheel.y);
  ofRect(body.x+body.width+ppi*3/32, body.y, wheel.x, wheel.y);
  /*ofSetColor(white*.8);
  ofRect(body.x+(body.width-ppi*1.375)/2, body.y+body.height, 1.375*ppi, .25*ppi);
  ofRect(body.x+(body.width-ppi*1.375)/2+1.125*ppi, body.y+body.height, .25*ppi,1.375*ppi);
  ofSetColor(white*.5);
  ofRect(body.x+(body.width-ppi*1.375)/2+.125*ppi, body.y+body.height+.5*ppi, .875*ppi, 1.5*ppi);
  ofSetColor(orange);
  ofRect(body.x+(body.width-ppi*1.375)/2+.125*ppi, body.y+body.height+.50625*ppi, .375*ppi, .4375*ppi);
  ofRect(body.x+(body.width-ppi*1.375)/2+.125*ppi, body.y+body.height+(2.-.4375)*ppi, .375*ppi, .4375*ppi);
  ofRect(body.x+(body.width-ppi*1.375)/2+(1-.375)*ppi, body.y+body.height+(.5+(1.5-.4375)/2)*ppi, .375*ppi, .4375*ppi);*/
  ofPopMatrix();
  ofDisableSmoothing();
  
  if(!frontIsClear(frontCheckDist)) ofSetColor(255, 0, 0);
  else ofSetColor(0, 255, 0);
  ofPoint ps = pointAlongBearing(frontCheckDist+wheel.y/2);
  ofCircle(ps.x, ps.y, 5);
  
  if(!leftIsClear(leftCheckDist)) ofSetColor(255, 0, 0);
  else ofSetColor(0, 255, 0);
  ps = pos+bearing.ortho().unit()*w/2-bearing.unit()*w/2+bearing.unit().rotate(270)*leftCheckDist;
  ofCircle(ps.x, ps.y, 5);
  
  for (unsigned int i=0; i<bdy.size(); i++) {
    ofSetColor(yellow);
    ofVector t=bdy[i];
    ofLine(pos, pos+t.rotate(360-bearing.absoluteAngle()));
  }
}
