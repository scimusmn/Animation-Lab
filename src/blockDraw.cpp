/*
 *  blockDraw.cpp
 *  robotBlocks_redux
 *
 *  Created by Exhibits on 1/31/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "blockDraw.h"
#include "robotConfig.h"

extern int TITLE_HEIGHT;

void glBezier3f(double Ax,double Bx, double Cx,double Ay,double By, double Cy){
	double a=1.0;
	double b=1.0-a;
	double x,y;
	double steps=10;
	for (int i=0; i<steps+1; i++) {
		x=Ax*a*a+Bx*2*a*b+Cx*b*b;
		y=Ay*a*a+By*2*a*b+Cy*b*b;
		glVertex2d(x,y);
		a-=1/steps;
		b=1.0-a;
	}
}

void glWideCurve(double Ax,double Bx, double Cx,double Ay,double By, double Cy,double sp,double r, double g, double bl, double aug,bool top){
	//bool up=true;
	double strt=.8;
	double a=1.0;
	double b=1.0-a;
	double x,y;
	double steps=10;
	for (int i=0; i<steps+1; i++) {
		x=Ax*a*a+Bx*2*a*b+Cx*b*b;
		y=Ay*a*a+By*2*a*b+Cy*b*b;
		if(!top) glColor4f(r+aug,g+aug,bl+aug,strt); else glColor4f(r,g,bl,0);
		glVertex2d(x,y+sp);
		if(!top) glColor4f(r,g,bl,0); else glColor4f(r+aug,g+aug,bl+aug,strt);
		glVertex2d(x,y);
		a-=1/steps;
		b=1.0-a;
	}
}

void notchVertices(float x, float y, float w, float h){
	if(cfg().defaultColor){
		ofVertex(x, y);
		ofVertex(x+w/2, y+h);
		ofVertex(x+w, y);
	}
	else {
		ofEnableSmoothing();
		ofVertex(x, y);
		ofVertex(x+w/4, y+h);
		ofVertex(x+w-w/4, y+h);
		ofVertex(x+w,y);
		ofDisableSmoothing();
	}
}

void drawBlock(double x, double y, double w, double h){
  double unit=TITLE_HEIGHT/4;
  ofBeginShape();{
    ofVertex(x+unit, y);
	notchVertices(x+2*unit,y,unit*2,unit);
    ofVertex(x+w, y);
    ofVertex(x+w, y+h);
	notchVertices(x+4*unit,y+h,-unit*2,unit);
    ofVertex(x, y+h);
    ofVertex(x, y+unit);
    ofVertex(x+unit, y);
  }
  ofEndShape();
};



void drawBaseBlock(double x, double y, double w, double h, double butX, double butY){
	double unit=TITLE_HEIGHT/4;
  ofBeginShape();{
    ofVertex(x, y);
    ofVertex(x, y+h);
	notchVertices(x+2*unit,y+h,2*unit,unit);
    ofVertex(x+w-butX, y+h);
    ofVertex(x+w+(butY-h)-butX, y+butY);
    ofVertex(x+w-(butY-h), y+butY);
    ofVertex(x+w, y+h);
    ofVertex(x+w, y);
  }
  ofEndShape();
}

void drawButtonSpace(double x, double y, double w, double h,double off)
{
  ofVertex(x, y);
  ofVertex(x, y+h-off);
  ofVertex(x+off, y+h);
  ofVertex(x+w-off, y+h);
  ofVertex(x+w, y+h-off);
  ofVertex(x+w, y);
}


/*void shadeCircleSlice(double x, double y, double ir, double oR,int beg,int end, double strt, double r,double g, double b,double shBot,double shTop){
	for ( int i = end+1; i >= beg-1; i-- ){
		float perc2 = shBot*cos(i*M_PI/(180))-shTop*sin(i*M_PI/(180));
		glColor4f(r+perc2,g+perc2,b+perc2,strt);
		glVertex2d(x+ir*cos(i*M_PI/(180)),y+ir*sin(i*M_PI/(180)));
		glColor4f(r,g,b,0);
		glVertex2d(x+oR*cos(i*M_PI/(180)),y+oR*sin(i*M_PI/(180)));
	}
}

void drawCircleSeg(double x, double y, double ir, double oR,int beg,int end, double r,double g, double b){
	glBegin(GL_QUAD_STRIP);
	for ( int i = end+1; i >= beg-1; i-- ){
		glColor4f(r,g,b,1);
		glVertex2d(x+ir*cos(i*M_PI/(180)),y+ir*sin(i*M_PI/(180)));
		glVertex2d(x+oR*cos(i*M_PI/(180)),y+oR*sin(i*M_PI/(180)));
	}
	glEnd();
}*/

void drawBigBlock(int x, int y, int w, int h, int xI, int yI, int yB)
{
  double unit=TITLE_HEIGHT/4;
  ofBeginShape();{
    ofVertex(x+unit, y);
    notchVertices(x+2*unit,y,2*unit,unit);
    ofVertex(x+w, y);
    ofVertex(x+w, y+yI);
	notchVertices(x+xI+4*unit,y+yI,-2*unit,unit);
    ofVertex(x+xI+unit, y+yI);
    ofVertex(x+xI, y+yI+unit);
    ofVertex(x+xI, y+yB);
	notchVertices(x+xI+2*unit,y+yB,2*unit,unit);
    ofVertex(x+w, y+yB);
    ofVertex(x+w, y+h);
	notchVertices(x+4*unit,y+h,-2*unit,unit);
    ofVertex(x, y+h);
    ofVertex(x, y+unit);
    ofVertex(x+unit, y);
  }
  ofEndShape();
}
