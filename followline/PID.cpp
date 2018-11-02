#include "Arduino.h"
#include "PID.h"

PID::PID(){
  
}
int PID::LineTrackingControl (float actualValue, float desiredValue){
  double error = desiredValue-actualValue; //hibameghatározás,  hol van hol kell lennie
  memcpy(errSum_array, errSum_array+1, 9*sizeof(double)); //10 elemű tömbben elcsúsztatja az értékeket
  errSum_array[9]=(error); // beteszi az utolsó hibát
  errSum=0;
  for(int i=0; i<9; i++) errSum+=errSum_array[i];
  errSum/=10; //átlagolás
  
  double dErr = (error - lastErr); //deriválótag errora
  
  Output = kp*error + ki * errSum+ kd *dErr; //output
  
  lastErr = error; //aktuális error eltárolása
  return Output;
}

PID pid = PID();
