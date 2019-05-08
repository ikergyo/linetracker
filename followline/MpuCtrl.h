#ifndef MpuCtrl_h
#define MpuCtrl_h
#include <Wire.h>

class MpuCtrl
{
public:
  void Setup();
  void setUpMPU();
  void callibrateGyroValues();
  void readAndProcessGyroData();
  void getGyroValues();
  void calculateAngle();
  float getAngleZ();
  void printData();

private: 
  long gyroXCalli;
  long gyroYCalli;
  long gyroZCalli;
  long gyroXPresent;
  long gyroYPresent; 
  long gyroZPresent;
  long gyroXPast;
  long gyroYPast;
  long gyroZPast;
  float rotX;
  float rotY;
  float rotZ;
  float angelX;
  float angelY;
  float angelZ;
  long timePast;
  long timePresent;
  
};
extern MpuCtrl mpuCtrl;
#endif
