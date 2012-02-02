#include <Servo.h>
#include <Wire.h> 
 
Servo tail;  
Servo flip;
 
int butPin=5;
int tailPin=9;
int flipPin=10;
int wrigPin=11;
int times=0;
int tailPos=90;
int flipPos=90;
bool tailDir=true;
bool flipDir=true;
bool playing=false;
long tailTimer=0;
long flipTimer=0;

long timer=0;
 
void setup() 
{ 
  Wire.begin();
  tail.attach(tailPin);
  flip.attach(flipPin);
  pinMode(wrigPin,OUTPUT);
  pinMode(19,INPUT);
} 
 
void loop() 
{
