#include "Arduino.h"
#include "Sensor.h"
#include "Motor.h"
#include "PID.h"
#include "Timer_Config.h"
#include "ServoCtrl.h"
#include "SonarCtrl.h"
#include "MpuCtrl.h"


boolean moveState = false;
boolean obstacleCourse = true;
boolean initObstacleState=true;

//Searchingstate values
boolean searchingState = false;
boolean searchingStateLeft = false;
boolean searchingStateRight = false;
int searchingValue = 10;
int searchingLimitLeft = 180; //90 + 180
int searchingLimitRight = 0;
int obstacleLimitInSearching = 50;

//Haladás obstacleCourse módban
boolean followingDirection = false;
float angleLimitPID = 3.0f;
float speedMultiplierPID = 10.0f;
int obstacleLimitInFollowing = 50;
float changeAngle = 1.0f;

bool mpuCanMeasure = false;

float absoluteAngle=0;

boolean IR_read;
int base_rpm = 120;

int onTime = 0;
void setup() {

  sensor.Setup();
  motor.Setup();
  servoCtrl.Setup();
  sonarCtrl.Setup();
  initTimer1();
  motor.setLeftRotDirection(true);
  motor.setRightRotDirection(true);
}

void loop() {
  if(initObstacleState)
  {    
    mpuCtrl.Setup();
    initObstacleState=false;
    followingDirection=true;
  }
  if(mpuCanMeasure)
  {
    mpuCtrl.readAndProcessGyroData();
    mpuCanMeasure=false;    
  }
}
void stopCar(){
  motor.setLeftVelocity(0);
  motor.setRightVelocity(0);
}

//timer configba
ISR(TIMER1_OVF_vect){
  TCNT1 = 65535-20000;
  
  IR_read = true;
  sensor.readSensors();
  //sensor.writeDatas(sensor.sens);
  
  if (obstacleCourse)
  {
    
    if(sonarCtrl.inMeasure)
    {
      return;
    }
    servoCtrl.update();
    
    if(!servoCtrl.isRotating())
    {
      sonarCtrl.getMeasure();
    }
    mpuCanMeasure=true;

    if(searchingState)
    {
      stopCar();
      if(searchingStateLeft)
      {
        if(sonarCtrl.dataIsHot && !servoCtrl.isRotating()) //Ha épp jött adat a sonartól
        { 
          if(servoCtrl.getAngle()+searchingValue <= searchingLimitLeft)
          {
            servoCtrl.setAngle(servoCtrl.getAngle()+searchingValue);
            if(sonarCtrl.getActualValue() < obstacleLimitInSearching) //megvan az irány
            {
              absoluteAngle = servoCtrl.getAngle() + mpuCtrl.getAngleZ() - 90;
              searchingState = false;
              followingDirection = true;
              servoCtrl.setAngle(90);
            }
          }else{
            searchingStateLeft =false;
            searchingStateRight = true;
          }
        }
      }
      else if(searchingStateRight)
      {
        if(sonarCtrl.dataIsHot && !servoCtrl.isRotating()) //Ha épp jött adat a sonartól
        { 
          if(servoCtrl.getAngle()-searchingValue >= searchingLimitRight)
          {
            servoCtrl.setAngle(servoCtrl.getAngle()-searchingValue);
            if(sonarCtrl.getActualValue() < obstacleLimitInSearching) //megvan az irány
            {
              absoluteAngle = servoCtrl.getAngle() + mpuCtrl.getAngleZ() - 90;
              searchingState = false;
              followingDirection = true;
              servoCtrl.setAngle(90);
            }
          }
          else
          {
            searchingStateRight = false;
            searchingStateLeft = true;
          }
        }
      }
    }
    else if (followingDirection)
    {
      if (absoluteAngle - mpuCtrl.getAngleZ() > angleLimitPID)
      {
        motor.turnLeft(base_rpm);
        searchingStateRight = true;
      }
      else if (absoluteAngle - mpuCtrl.getAngleZ() < -angleLimitPID)
      {
        motor.turnRight(base_rpm);  
        searchingStateLeft = true;
      }
      else
      {
        int rpm = speedMultiplierPID * pid.LineTrackingControl(absoluteAngle, mpuCtrl.getAngleZ() );
        int R_rpm = base_rpm-rpm;
        if (R_rpm <0) R_rpm =0;
        if (R_rpm >255) R_rpm=255;
        int L_rpm = base_rpm+rpm;
        if (L_rpm <0) L_rpm = 0;
        if (L_rpm >255) L_rpm = 255;

        motor.setLeftRotDirection(true);
        motor.setRightRotDirection(true);
        motor.setLeftVelocity(L_rpm);
        motor.setRightVelocity(R_rpm);

        //ha akadályt talál előrefele
        if (!servoCtrl.isRotating() && sonarCtrl.dataIsHot && sonarCtrl.getActualValue() < obstacleLimitInFollowing)
        {
          searchingState = true;
          if(searchingStateLeft == false && searchingStateRight == false) //Ha valaki már setelve lett akkor ne írja felül.
          {
            searchingStateLeft = true;
            searchingStateRight = true;
          }
        }
        
        //próbál az eredeti irányba tartani
        else if ( !servoCtrl.isRotating() )
        {
          if (servoCtrl.getAngle() == 90)
          {
            if(absoluteAngle < -changeAngle / 2)
              servoCtrl.setAngle(90+changeAngle);
            else if(absoluteAngle > changeAngle /2)
              servoCtrl.setAngle(90-changeAngle);
          }
          else
          {
            if(sonarCtrl.dataIsHot && sonarCtrl.getActualValue() > obstacleLimitInFollowing)
              absoluteAngle += servoCtrl.getAngle() - 90;
            servoCtrl.setAngle(90);
          }
        }
               
      }
    }
    
  } else {
    if(!moveState && sensor.needToStart()){
      moveState = true; 
    }
    if(moveState){
      if(sensor.needToStop()){
        moveState = false;
        obstacleCourse = true;
        initObstacleState=true;
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
