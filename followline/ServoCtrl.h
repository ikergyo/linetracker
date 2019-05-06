#ifndef ServoCtrl_h
#define ServoCtrl_h

#include <Servo.h>
#define SERVO_PIN 9

class ServoCtrl
{
  boolean rotating;
  int waitCycle;
  int actualAngle;
  
public:
  ServoCtrl();
  void update();
  boolean isRotating();
  void setAngle(int angle);
  int getAngle();
};

extern ServoCtrl servoCtrl;
#endif
