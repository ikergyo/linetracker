#include "Arduino.h"

void initTimer1(){
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 65535 - 20000; //100HZ
  TCCR1B |= B00000010;
  TIMSK1 |= (1<<TOIE1);
}
