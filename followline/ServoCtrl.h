
#ifndef ServoCtrl_h
#define ServoCtrl_h

#include <Servo.h>
#define SERVO_PIN 9

class ServoCtrl
{
public:
  void Setup();
  void update();
  boolean isRotating();
  void setAngle(int angle);
  int getAngle();
  int readOut();

private: 
  boolean rotating;
  int waitCycle;
  int actualAngle;
  
};
extern ServoCtrl servoCtrl;
#endif
