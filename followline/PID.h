#ifndef PID_h
#define PID_h

class PID{
  public:
    PID();
    int LineTrackingControl (float actualValue, float desiredValue);
  private:
    double Output;
    double errSum, lastErr;
    double errSum_array[10] = {0,0,0,0,0,0,0,0,0,0};

    double kp=3;

    double ki=0;
    double kd=0;
};

extern PID pid;

#endif
