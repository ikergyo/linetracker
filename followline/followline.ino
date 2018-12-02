#include "Arduino.h"
#include "Sensor.h"
#include "Motor.h"
#include "PID.h"
#include "Timer_Config.h"

boolean moveState = false;
boolean IR_read;
int base_rpm = 120;

int onTime = 0;
void setup() {

  // put your setup code here, to run once:
  sensor.Setup();
  motor.Setup();
  initTimer5();
  motor.setLeftRotDirection(true);
  motor.setRightRotDirection(true);
}

void loop() {

}

//timer configba
ISR(TIMER5_OVF_vect){

  IR_read = true;

  sensor.readSensors();
  //sensor.writeDatas(sensor.sens);
  if(!moveState && sensor.needToStart()){
    moveState = true; 
  }
  if(moveState){
    if(sensor.needToStop()){
      moveState = false;
      motor.setLeftVelocity(0);
      motor.setRightVelocity(0);
    }else{
      
      int mainBit = sensor.getMainBit();
      //Serial.println(mainBit);
      int rpm = pid.LineTrackingControl(mainBit*10,40.0);
    
      int R_rpm = base_rpm-rpm;
      if (R_rpm <0) R_rpm =0;
      if (R_rpm >255) R_rpm=255;
      int L_rpm = base_rpm+rpm;
      if (L_rpm <0) L_rpm = 0;
      if (L_rpm >255) L_rpm = 255;
    
      motor.setLeftVelocity(L_rpm);
     
      motor.setRightVelocity(R_rpm);
  
      TCNT5 = 65535-20000;
    }
  }
}
