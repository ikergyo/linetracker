#ifndef SonarCtrl_h
#define SonarCtrl_h

#define echoPin 11
#define trigPin 10

class SonarCtrl{
  
  
public:
  void Setup();
  long getActualValue();
  void getMeasure();
  boolean dataIsHot;
  int obstacleLimitFirst;
  int obstacleLimitInBadDirection;
  int obstacleLimitInSearching;
  bool inMeasure;
  
private:
  int cycleCounter;
  long distance;
  long duration;
  int maximumRange;
  int minimumRange;

};

extern SonarCtrl sonarCtrl;

#endif
