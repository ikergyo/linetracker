#ifndef SonarCtrl_h
#define SonarCtrl_h

#define echoPin 10
#define trigPin 9

class SonarCtrl{
  
  
public:
  void Setup();
  long getActualValue();
  void getMeasure();
private:
  int cycleCounter;
  long distance;
  long duration;
  int maximumRange;
  int minimumRange;
};

extern SonarCtrl sonarCtrl;

#endif
