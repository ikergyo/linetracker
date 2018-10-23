#include "Arduino.h"
#include "Sensor.h"
#include "Motor.h"


int onTime = 0;
void setup() {
  // put your setup code here, to run once:
  sensor.Setup();
  motor.Setup();
}

void loop() {

  onTime = millis();
   
  if(onTime > 5000){
   motor.setRightVelocity(0);
   motor.setLeftVelocity(0);
  }else{
    motor.setLeftRotDirection(true);
    motor.setLeftVelocity(200);
  }
  /*
  sensor.readSensors();
  sensor.writeDatas();
  delay(50);*/
}
