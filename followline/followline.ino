#include "Arduino.h"
#include "Sensor.h"
#include "Motor.h"
#include "PID.h"
#include "Timer_Config.h"
#include "ServoCtrl.h"
#include "SonarCtrl.h"


boolean moveState = false;
boolean obstacleCourse = true;
boolean IR_read;
int base_rpm = 120;

int onTime = 0;
void setup() {

  // put your setup code here, to run once:
  sensor.Setup();
  motor.Setup();
  servoCtrl.Setup();
  sonarCtrl.Setup();
  initTimer1();
  motor.setLeftRotDirection(true);
  motor.setRightRotDirection(true);
}

void loop() {

}

//timer configba
ISR(TIMER1_OVF_vect){

  IR_read = true;
  
  sensor.readSensors();
  TCNT1 = 65535-20000;
  //sensor.writeDatas(sensor.sens);
  
  if (obstacleCourse){
    servoCtrl.update();
    sonarCtrl.getMeasure();
  } else {
    if(!moveState && sensor.needToStart()){
      moveState = true; 
    }
    if(moveState){
      if(sensor.needToStop()){
        moveState = false;
        obstacleCourse = true;
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
     }
   }
  }
  
}
