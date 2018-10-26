#include "Arduino.h"

void initTimer5(){
  TCCR5A = 0;
  TCCR5B = 0;
  TCNT5 = 65535 - 20000; //100HZ
  TCCR5B |= B00000010;
  TIMSK5 |= (1<<TOIE5);
}
