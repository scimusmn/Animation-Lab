byte forb1 = FORWARD;
byte forb2 = FORWARD;

if(analogRead(0)>200){
    timer2 = millis()+1500;
    turn = true;}


Wire.requestFrom(0x50 >> 1,1);
byte data = Wire.receive();
for(int i=0;i<5;i++){
	side +=bitRead(data,i)*(i-2);
}

if(abs(side)>=2){
    side=(side/(abs(side)))*3;}
if(bitIsSet(data,2)){
    side=0;}

/*if(side>0){
 Serial.print("Left ");
 Serial.print(16+side*5);
 Serial.print("\n");
 delay(100);}
 if(side<0){
 Serial.print("Right \n");
 Serial.print(16+side*5);
 Serial.print("\n");
 delay(100);}
 if(side==0){
 Serial.print("Stop \n");
 delay(100);}*/

/*if(side<0){
 forb1=FORWARD;
 forb2=BACKWARD;}
 else if(side>0){
 forb2=FORWARD;
 forb1=BACKWARD;}*/

if(turn&&timer2>millis()){
	motor1.setSpeed(96);     // set the speed to correction speeds
	motor2.setSpeed(96);
	motor3.setSpeed(150);
	motor4.setSpeed(150);
	
	motor1.run(FORWARD);      // turn it on going forward
	motor2.run(FORWARD);
	motor3.run(BACKWARD);      // turn it on going forward
	motor4.run(BACKWARD);
}

else if(turn&&timer2<millis()){
    turn=false;
    side=0;
}
else {
	motor1.setSpeed(96+side*30);     // set the speed to correction speeds
	motor2.setSpeed(96+side*30);
	motor3.setSpeed(96-side*30);
	motor4.setSpeed(96-side*30);
	
	motor1.run(forb1);      // turn it on going forward
	motor2.run(forb2);
	motor3.run(forb1);      // turn it on going forward
	motor4.run(forb2);}
