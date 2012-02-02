/*
 *  progressReader.cpp
 *  arenaBot
 *
 *  Created by Exhibits on 11/29/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "progressReader.h"

progRead::progRead()
{
  checkTimer.set(0.01);
  checkTimer.run();
  fd=0;
  //start();
}

progRead::~progRead()
{
  close(fd);
  stop();
}

void progRead::reset()
{
  percent=0;
  lineCount=0;
  message="";
  rep="";
  start();
}

double progRead::percentDone()
{
  return percent;
}

string progRead::report()
{
  return rep;
}

int readByteFromPipe(int fd)
{
  unsigned char tmpByte[1];
	memset(tmpByte, 0, 1);
  
  int nRead = read(fd, tmpByte, 1);
  if(nRead < 0){
    return -1;
  }
  if(nRead == 0)
    return 0;
  
  return (int)(tmpByte[0]);
}

void progRead::start(){
  startThread(true, false);   // blocking, verbose
}

void progRead::stop(){
  stopThread();
}

//--------------------------
void progRead::threadedFunction(){
  
  while( isThreadRunning() != 0 ){
    checkFile();
  }
}

void progRead::checkFile()
{
  if(!fd) fd = open(ofToDataPath("arduino_make/progress").c_str(), O_RDONLY);
  int t[1];
  
  if((t[0]=readByteFromPipe(fd))!='\n'&&t[0]>0){
    message+=string((char*)(t));
  }
  else if(t[0]=='\n'){
    //cout << message << endl;
    if(message=="Compiling..."){
      rep=message;
    }
    else if(message=="Contacting programmer..."){
      rep=message;
    }
    message="";
  }
  vector<string> token=ofSplitString(message, " |");
  if(token.size()>1){
    if(token[0]=="Writing"){
      rep="Writing...";
      percent=token[1].length()/50.;
      //cout << percent*100 << "%\n";
    }
    if(token[0]=="Reading"){
      rep="Preparing chip.";
    }
  }
}