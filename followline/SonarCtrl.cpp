#include "Arduino.h"
#include "SonarCtrl.h"

void SonarCtrl::Setup(){
  maximumRange = 200;
  minimumRange = 0;
  cycleCounter=0;
  distance = 0;
  duration = 0;

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
long SonarCtrl::getActualValue(){
  return distance;
}

void SonarCtrl::getMeasure(){
  if(cycleCounter == 5){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
  
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
  
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
  
    distance = duration/58.2;
  
    if(distance >= maximumRange || distance <= minimumRange){
      //Serial. println("-1");
      distance = 0;
    }
    else{
      //Serial.println(distance);
    }
    cycleCounter = 0;
  }else{
    cycleCounter++;
  }
  
}

SonarCtrl sonarCtrl = SonarCtrl(); 
