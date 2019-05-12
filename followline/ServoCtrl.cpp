
#include "Arduino.h"
#include "ServoCtrl.h"
#include <math.h>
Servo servo;

void ServoCtrl::Setup(){
    rotating = false;
    waitCycle = 0;
    actualAngle = 90;
    servo.attach(SERVO_PIN);
    Serial.println("Beléptem");
    setAngle(90);
}
void ServoCtrl::update(){
    if(waitCycle > 0)
      waitCycle--;
    if (waitCycle == 0)
        rotating = false;
}

boolean ServoCtrl::isRotating(){
    return rotating;
}
  
void ServoCtrl::setAngle(int angle){
    if(!rotating){
      if (angle > 175)
        angle = 175;
      if (angle < 5)
        angle = 5;
      rotating = true;
      waitCycle = abs(angle-actualAngle) / 5 + 1;
      actualAngle = angle;
      servo.write(angle);

   }
}

int ServoCtrl::getAngle(){
  return actualAngle;
}


ServoCtrl servoCtrl = ServoCtrl();
