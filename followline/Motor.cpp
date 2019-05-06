#include "Arduino.h"
#include "Motor.h"


Motor::Motor(){
}

void Motor::Setup(){
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void Motor::setLeftRotDirection(boolean forward){
  if(forward){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }else{
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
}

void Motor::setRightRotDirection(boolean forward){
  if(forward){
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }else{
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
}

void Motor::setLeftVelocity(byte velocity){
  analogWrite(enA, velocity);
}
void Motor::setRightVelocity(byte velocity){
  analogWrite(enB, velocity);
}

void Motor::turnLeft(byte velocity){
  setLeftVelocity(velocity);
  setRightVelocity(velocity);
  setLeftRotDirection(false);
  setRightRotDirection(true);
}

void Motor::turnRight(byte velocity){
  setLeftVelocity(velocity);
  setRightVelocity(velocity);
  setLeftRotDirection(true);
  setRightRotDirection(false);
}

void Motor::goForward(byte velocity){
  setLeftVelocity(velocity);
  setRightVelocity(velocity);
  setLeftRotDirection(true);
  setRightRotDirection(true);
}

Motor motor = Motor();
