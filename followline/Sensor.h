#ifndef Sensor_h
#define Sensor_h

#define S1 A0
#define S2 A1
#define S3 A2
#define S4 A3
#define S5 A4
#define S6 A5
#define S7 A6
#define S8 A7
#define S9 A8

#define SD1 45
#define SD2 47
#define SD3 49
#define SD4 51
#define SD5 53
#define SD6 46
#define SD7 48
#define SD8 50
#define SD9 52

#define SENSOR_NUM 9
#define BUFFER_NUM 100

class Sensor
{
  public:
    Sensor();
    byte sens[SENSOR_NUM];
    byte bufferSens[BUFFER_NUM][SENSOR_NUM];
    void Setup();
    void readSensors();
    void writeDatas();
    float sensorIndexAvarage();
    int getRightBit(byte sensData[]); 
    int getLeftBit(byte sensData[]);
      
  private:
    void loadLast(); 
    
    void addBuffer(byte actualSensData[]);
    void bufferCopy(boolean bufferToSens);
};

extern Sensor sensor;
/*extern byte sens[SENSOR_NUM] sens;
extern byte bufferSens[BUFFER_NUM][SENSOR_NUM];*/

#endif
