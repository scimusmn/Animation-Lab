/******************************************
*
*  Line Following Robot Test Program
*  Aaron Heidgerken
*
*   Arduino analog input 5 - I2C SCL
*   Arduino analog input 4 - I2C SDA
*******************************************/


#include <AFMotor.h>
#include <Wire.h>

AF_DCMotor motor1(1, MOTOR12_64KHZ); // create motor #2, 64KHz pwm
AF_DCMotor motor2(2, MOTOR12_64KHZ);
AF_DCMotor motor3(3, MOTOR12_1KHZ); // create motor #2, 64KHz pwm
AF_DCMotor motor4(4, MOTOR12_1KHZ);

signed int side =0;
long timer=0;
long timer2=0;

bool turn=false;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Wire.begin();
  delay(1000);
}

bool bitIsSet(byte dta, int pos){
  return ((dta >> pos) % 2);
}

void loop() {
