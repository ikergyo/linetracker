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
#define BUFFER_NUM 50
#define SENSOR_START_LIMIT 5
#define STOP_LIMIT 10

#define LANE_CHANGE_LIMIT 25 //Mennyi kell ahhoz, hogy sávot váltson.
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
    boolean needToStop();
    boolean needToStart();
    int getMainBit();
      
  private:
    void loadLast(); 
    int getRightBit(byte sensData[]); 
    int getLeftBit(byte sensData[]);
    int getLaneChange();
    boolean getDifferenceWithZero(byte sensData[]);
    boolean getDifference(byte sensData[]);
    int getLastNormalBufferIndex(int last);
    boolean allSensIsZero(byte sensData[]);
    boolean allSensIsOne(byte sensData[]);
    boolean isThisY(byte sensData[]);
    void addBuffer(byte actualSensData[]);
    void bufferCopy(boolean bufferToSens);
};

extern Sensor sensor;
/*extern byte sens[SENSOR_NUM] sens;
extern byte bufferSens[BUFFER_NUM][SENSOR_NUM];*/

#endif
