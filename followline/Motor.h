#ifndef Motor_h
#define Motor_h

#define enA 7
#define enB 6
#define in1 5
#define in2 4
#define in3 3
#define in4 2

class Motor
{
  public:
    Motor();
    void Setup();
    void setRightVelocity(byte velocity);
    void setLeftVelocity(byte velocity); 
    void setRightRotDirection(boolean forward);
    void setLeftRotDirection(boolean forward);
      
  private:
    void loadLast(); 
    void bufferCopy(boolean bufferToSens);
};

extern Motor motor;

#endif
