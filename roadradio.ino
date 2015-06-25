/* 
* //////////////////////////////////////////////////
* //making sense of the Parallax PIR sensor's output
* //////////////////////////////////////////////////
*
* Switches a LED according to the state of the sensors output pin.
* Determines the beginning and end of continuous motion sequences.
*
* @author: Kristian Gohlke / krigoo (_) gmail (_) com / http://krx.at
* @date: 3. September 2006 
*
* kr1 (cleft) 2006 
* released under a creative commons "Attribution-NonCommercial-ShareAlike 2.0" license
* http://creativecommons.org/licenses/by-nc-sa/2.0/de/
*
*
* The Parallax PIR Sensor is an easy to use digital infrared motion sensor module. 
* (http://www.parallax.com/detail.asp?product_id=555-28027)
*
* The sensor's output pin goes to HIGH if motion is present.
* However, even if motion is present it goes to LOW from time to time, 
* which might give the impression no motion is present. 
* This program deals with this issue by ignoring LOW-phases shorter than a given time, 
* assuming continuous motion is present during these phases.
* 
*/

/////////////////////////////
//VARS
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 1; 

//the time when the sensor outputs a low impulse
long unsigned int lowIn; 
long unsigned int lowIn2; 

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 10; 

boolean lockLow = true;
boolean takeLowTime; 
boolean lockLow2 = true;
boolean takeLowTime2; 

float timestamp1 = 0.0;
float timestamp2 = -1.0;
float distance = 1.0;
float velocity = 1000.0;

int pirPin = 3; 
int pirPin2 = 2; //the digital pin connected to the PIR sensor's output
int ledPin = 13;


/////////////////////////////
//SETUP
void setup(){
Serial.begin(9600);
pinMode(pirPin, INPUT);
pinMode(ledPin, OUTPUT);
digitalWrite(pirPin, LOW);
//give the sensor some time to calibrate
Serial.print("calibrating sensor ");
for(int i = 0; i < calibrationTime; i++){
Serial.print(".");
delay(1000);
}
Serial.println("done");
Serial.println("SENSOR 1 ACTIVE");
delay(50);
Serial.begin(9600);
pinMode(pirPin2, INPUT);
pinMode(ledPin, OUTPUT);
digitalWrite(pirPin2, LOW);
//give the sensor some time to calibrate
Serial.print("calibrating sensor ");
for(int i = 0; i < calibrationTime; i++){
Serial.print(".");
delay(1000);
}
Serial.println("done");
Serial.println("SENSOR 2 ACTIVE");
delay(50);
}


////////////////////////////
//LOOP
void loop(){
//Serial.print(timestamp2-timestamp1);

if(digitalRead(pirPin) == HIGH){
digitalWrite(ledPin, HIGH); //the led visualizes the sensors output pin state
if(lockLow){ 
//makes sure we wait for a transition to LOW before any further output is made:
lockLow = false; 
Serial.println("---");
Serial.print("motion 1 detected at ");
timestamp1 = millis();
Serial.print(timestamp1);
Serial.println(" milsec"); 
delay(1);
} 
takeLowTime = true;
}

if(digitalRead(pirPin) == LOW){ 
digitalWrite(ledPin, LOW); //the led visualizes the sensors output pin state

if(takeLowTime){
lowIn = millis(); //save the time of the transition from high to LOW
takeLowTime = false; //make sure this is only done at the start of a LOW phase
}
//if the sensor is low for more than the given pause, 
//we assume that no more motion is going to happen
if(!lockLow && millis() - lowIn > pause){ 
//makes sure this block of code is only executed again after 
//a new motion sequence has been detected
lockLow = true; 
//Serial.print("motion 1 ended at "); //output
//Serial.print((millis() - pause));
//Serial.println(" milsec");
delay(1);
}
}
if(digitalRead(pirPin2) == HIGH){
digitalWrite(ledPin, HIGH); //the led visualizes the sensors output pin state
if(lockLow2){ 
//makes sure we wait for a transition to LOW before any further output is made:
lockLow2 = false; 
Serial.println("---");
Serial.print("motion 2 detected at ");
timestamp2 = millis();
Serial.print(timestamp2);
Serial.println(" milsec"); 
delay(1);
} 
takeLowTime2 = true;
}

if(digitalRead(pirPin2) == LOW){ 
digitalWrite(ledPin, LOW); //the led visualizes the sensors output pin state

if(takeLowTime2){
lowIn2 = millis(); //save the time of the transition from high to LOW
takeLowTime2 = false; //make sure this is only done at the start of a LOW phase
}
//if the sensor is low for more than the given pause, 
//we assume that no more motion is going to happen
if(!lockLow2 && millis() - lowIn2 > pause){ 
//makes sure this block of code is only executed again after 
//a new motion sequence has been detected
lockLow2 = true; 
//Serial.print("motion 2 ended at "); //output
//Serial.print((millis() - pause));
//Serial.println(" milsec");
delay(1);
}

}

if(((distance/(timestamp2 - timestamp1)))>velocity){
Serial.print("#S|OPENALERT|[");
Serial.println("]#");
}

delay(10);
timestamp2 = -1;
timestamp1 = 0;

}




