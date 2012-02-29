/******************************************
*
*  Line Following Robot Test Program
*  Aaron Heidgerken
*
*   Arduino analog input 5 - I2C SCL
*   Arduino analog input 4 - I2C SDA
*******************************************/


//#include <AFMotor.h>
#include <Wire.h>

signed int side =0;
long timer=0;
long timer2=0;

bool turn=false;

void setup() {
  pinMode(13,OUTPUT);
}

void loop() {
