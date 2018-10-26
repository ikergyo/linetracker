#include "Arduino.h"
#include "Sensor.h"
#include "Motor.h"
#include "PID.h"

boolean IR_read;
int base_rpm = 120;

int onTime = 0;
void setup() {
  // put your setup code here, to run once:
  sensor.Setup();
  motor.Setup();
}

void loop() {
  sensor.readSensors();
  sensor.writeDatas();
}

//timer configba
ISR(TIMER5_OVF_vect){
  IR_read = true;

  int rpm = pid.LineTrackingControl(sensor.getRightBit()*10,40.0);

  int R_rpm = base_rpm + rpm;
  if (R_rpm <0) R_rpm =0;
  if (R_rpm >255) R_rpm=255;
  int L_rpm = base_rpm-rpm;
  if (L_rpm <0) L_rpm = 0;
  if (L_rpm >255) L_rpm = 255;

  motor.setLeftRotDirection(true);
  motor.setLeftVelocity(L_rpm);
  motor.setRightRotDirection(true);
  motor.setRightVelocity(R_rpm);
  
  
  TCNT5 = 65535-20000;
}
