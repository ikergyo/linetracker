
#include "Arduino.h"
#include "Sensor.h"


const int limitLine = 600;

int pureSens[SENSOR_NUM];
byte sens[SENSOR_NUM];
byte bufferSens[SENSOR_NUM];

Sensor::Sensor(){
}

void Sensor::Setup(){
 Serial.begin(9600);
 pinMode(SD1,OUTPUT);
 pinMode(SD2,OUTPUT);
 pinMode(SD3,OUTPUT);
 pinMode(SD4,OUTPUT);
 pinMode(SD5,OUTPUT);
 pinMode(SD6,OUTPUT);
 pinMode(SD7,OUTPUT);
 pinMode(SD8,OUTPUT);
 pinMode(SD9,OUTPUT);
 
 digitalWrite(SD1, HIGH);
 digitalWrite(SD2, HIGH);
 digitalWrite(SD3, HIGH);
 digitalWrite(SD4, HIGH);
 digitalWrite(SD5, HIGH);
 digitalWrite(SD6, HIGH);
 digitalWrite(SD7, HIGH);
 digitalWrite(SD8, HIGH);
 digitalWrite(SD9, HIGH);
}

void Sensor::readSensors(){

  pureSens[0]=analogRead(A0);
  pureSens[1]=analogRead(A1);
  pureSens[2]=analogRead(A2);
  pureSens[3]=analogRead(A3);
  pureSens[4]=analogRead(A4);
  pureSens[5]=analogRead(A5);
  pureSens[6]=analogRead(A6);
  pureSens[7]=analogRead(A7);
  pureSens[8]=analogRead(A8);
  int sensBitValue = 0;
  for(int i = 0; i < SENSOR_NUM; i++){
     
    sensBitValue = 0;
    if(pureSens[i] > limitLine){
      sensBitValue = 1;
    }
    
    sens[i] = sensBitValue;
  }
  loadLast();
  bufferCopy(false);
}

void Sensor::bufferCopy(boolean bufferToSens){
  if(bufferToSens){
    for(int i=0; i< SENSOR_NUM; i++){
      sens[i]=bufferSens[i];
    }
  }
  for(int i=0; i< SENSOR_NUM; i++){
    bufferSens[i]=sens[i];
  }
}
void Sensor::loadLast(){
  boolean hasOne = false;
  for(int i = 0; i < SENSOR_NUM; i++){
    if(sens[i] == 1){
      hasOne = true;
      break;
    }
  }
  if(!hasOne){
    bufferCopy(true);
  }
}
float Sensor::sensorIndexAvarage(){
  float sum = 0;
  float count = 0;
  for(int i=0; i<SENSOR_NUM;i++){
    if(sens[i] == 1){
      sum += i;
      count++;
    }
  }
  return sum/count;
}
int Sensor::getRightBit(){
  for(int i=0; i<SENSOR_NUM; i++){
    if(sens[i] == 1){
      return i;
    }
  }
  return SENSOR_NUM/2; //közepe
}
void Sensor::writeDatas(){

  for(int i = 0; i< SENSOR_NUM;i++){
    Serial.print(sens[i]);
  }
  Serial.println();
  
}

Sensor sensor = Sensor();
