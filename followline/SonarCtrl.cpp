#include "Arduino.h"
#include "SonarCtrl.h"

void SonarCtrl::Setup(){
  maximumRange = 300;
  minimumRange = 3;
  cycleCounter=0;
  distance = 0;
  duration = 0;
  obstacleLimitFirst = 45;
  obstacleLimitInBadDirection = 55;
  obstacleLimitInSearching = 60;
  dataIsHot = false;
  inMeasure=false;
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
long SonarCtrl::getActualValue(){
  return distance;
}

void SonarCtrl::getMeasure(){
  if(cycleCounter == 5){
    inMeasure=true;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
  
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
  
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    inMeasure=false;
    distance = duration/58.2;
  
    if(distance >= maximumRange) 
    {
      distance = maximumRange;
    }
    else if(distance <= minimumRange)
    {
      distance = minimumRange;
    }
    cycleCounter = 0;
    dataIsHot = true;
  }else{
    dataIsHot = false;
    cycleCounter++;
  }
  
}

SonarCtrl sonarCtrl = SonarCtrl(); 
