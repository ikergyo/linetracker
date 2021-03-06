
#include "Arduino.h"
#include "Sensor.h"


const int limitLine = 650;
const int laneChangeValue = 1;
const int neededRotateTimeForLaneChangeLIMIT = 90;

int pureSens[SENSOR_NUM];
boolean nowLaneChange = false;
boolean leftLaneChange = false;
boolean rightLaneChange = false;
int laneChangeBit = -1;
int neededRotateTimeForLaneChange = 0;
/*
byte sens[SENSOR_NUM];
byte bufferSens[BUFFER_NUM][SENSOR_NUM];
*/
//int bufferSensCount = 0;

Sensor::Sensor(){
}

void Sensor::Setup(){
 Serial.begin(9600);
 pinMode(SD1,OUTPUT);
 pinMode(SD2,OUTPUT);
 pinMode(SD3,OUTPUT);
 pinMode(SD4,OUTPUT);
 pinMode(SD5,OUTPUT);
 pinMode(SD6,OUTPUT);
 pinMode(SD7,OUTPUT);
 pinMode(SD8,OUTPUT);
 pinMode(SD9,OUTPUT);
 
 digitalWrite(SD1, HIGH);
 digitalWrite(SD2, HIGH);
 digitalWrite(SD3, HIGH);
 digitalWrite(SD4, HIGH);
 digitalWrite(SD5, HIGH);
 digitalWrite(SD6, HIGH);
 digitalWrite(SD7, HIGH);
 digitalWrite(SD8, HIGH);
 digitalWrite(SD9, HIGH);
}

void Sensor::readSensors(){

  pureSens[0]=analogRead(A0);
  pureSens[1]=analogRead(A1);
  pureSens[2]=analogRead(A2);
  pureSens[3]=analogRead(A3);
  pureSens[4]=analogRead(A4);
  pureSens[5]=analogRead(A5);
  pureSens[6]=analogRead(A6);
  pureSens[7]=analogRead(A7);
  pureSens[8]=analogRead(A8);
  int sensBitValue = 0;
  for(int i = 0; i < SENSOR_NUM; i++){
     
    sensBitValue = 0;
    if(pureSens[i] > limitLine){
      sensBitValue = 1;
    }
    
    sens[i] = sensBitValue;
  }
  //loadLast();
  //bufferCopy(false);
}

void Sensor::bufferCopy(boolean bufferToSens){
  if(bufferToSens){
    for(int i=0; i< SENSOR_NUM; i++){
      sens[i]=bufferSens[0][i];
    }
  }else  {
    addBuffer(sens);
  }
}
void Sensor::loadLast(){
  if(allSensIsZero(sens)){
    bufferCopy(true);
  }
}
float Sensor::sensorIndexAvarage(){
  float sum = 0;
  float count = 0;
  for(int i=0; i<SENSOR_NUM;i++){
    if(sens[i] == 1){
      sum += i;
      count++;
    }
  }
  return sum/count;
}
int Sensor::getRightBit(byte sensData[]){
  for(int i=0; i<SENSOR_NUM; i++){
    if(sensData[i] == 1){
      return i;
    }
  }
  return SENSOR_NUM/2; //k???zepe
}
int Sensor::getLeftBit(byte sensData[]){
  for(int i=SENSOR_NUM-1; i>-1; i--){
    if(sensData[i] == 1){
      return i;
    }
  }
  return SENSOR_NUM/2; //k??zepe
}
int Sensor::getMainBit(){
  if(nowLaneChange){
    if(allSensIsZero(sens)){
      nowLaneChange = false;
      createSens(SENSOR_NUM/2);
      bufferCopy(false);
      return SENSOR_NUM/2;
    }else{
      return laneChangeBit;
    }
  }
  if(leftLaneChange){
    if(allSensIsZero(sens)){
      if(neededRotateTimeForLaneChange < neededRotateTimeForLaneChangeLIMIT)
      {
        neededRotateTimeForLaneChange++;
        int tempIndex = SENSOR_NUM - laneChangeValue;
        createSens(tempIndex);
        bufferCopy(false);
        return tempIndex;
      }else{
        return SENSOR_NUM/2;
      }
    }else{
      neededRotateTimeForLaneChange = 0;
      leftLaneChange = false;
    }
  }else if(rightLaneChange){
    if(allSensIsZero(sens)){ //Ez ahhoz kell, hogy fordul??s ut??n egyenesen menjen. A Limittel lehet ??llitani, hogy mennyi ideig forduljon
      if(neededRotateTimeForLaneChange < neededRotateTimeForLaneChangeLIMIT)
      {
        neededRotateTimeForLaneChange++;
        createSens(laneChangeValue);
        bufferCopy(false);
        return laneChangeValue;
      }else{
        return SENSOR_NUM/2;
      }
    }else{
      neededRotateTimeForLaneChange = 0;
      rightLaneChange = false;
    }
  }
  
  if(allSensIsZero(sens)){
    //bufferCopy(false);
    laneChangeBit = getLaneChange();
    if(laneChangeBit != -1){
      createSens(laneChangeBit);
      bufferCopy(false);
      return laneChangeBit;
    }

    //Ha a mostani adatban l??tni, hogy van k??l??nbs??g, de m??g nincs eld??ntve akkor addig is az el??z?? olyan indexet adja vissza amikor m??g csak egy ??rt??k volt 
    //int lastNorm = getLastNormalBufferIndex(0);
    loadLast();
    return getRightBit(sens);
  }
  loadLast();
  bufferCopy(false);
  return getRightBit(sens);
}
int Sensor::getLaneChange(){
  int diffCount = 0;
  int lastDiff = 0;
  for (int i = 0; i< BUFFER_NUM; i++){
    int diff = getDifferenceWithZero(bufferSens[i]);
    if(diff != -1){
      //Teh??t ha ugyanannyi a k??l??nbs??g mint az el??z??n??l akkor ++
      /**if(lastDiff == diff){ 
         diffCount++;
      }else{
        diffCount=0;
      }*/
      diffCount++;
      //lastDiff = diff;
    }else{
      diffCount = 0; //Ez ahhoz kell, hogy egym??s ut??n legyen ennyi
    }

    if(diffCount >= LANE_CHANGE_LIMIT){
      
      int last = getLastNormalBufferIndex(i);
      writeDatas(bufferSens[last-1]);
      writeDatas(bufferSens[last]);
      //laneChange true lesz mert most elkezdi a s??vv??lt??st. Addig kell truenak lennie am??g ki nem null??z??dik aza aktu??lis ??rt??k
      nowLaneChange = true; 
      //addBufferbe az utols?? norm??lis ??rt??k. FIGYELEM EZZEL VAL??SZ??N??LEG EGYENESEN FOG MENNI.
      //addBuffer(bufferSens[last]);
      /*
       * Ekkor elvileg balra k??ne mennie
       */
      if(getLeftBit(bufferSens[last]) < getLeftBit(bufferSens[last-1])){ 
        Serial.println("Bal");
        leftLaneChange = true;
        return SENSOR_NUM - 1; //az??rt egy mert ??gy olyan mintha ezt l??tn??: {0,1,0,0,0,0,0,0,0} 
        
      }
      /*
       * Ekkor meg jobbra, mert nem balra,de lehetne csekkolni hogy: if(getRightBit(bufferSens[last]) < getRightBit(bufferSens[0]))
       */
      else{
        Serial.println("Jobb");
        rightLaneChange = true;
        return 0; //az??rt egy mert ??gy olyan mintha ezt l??tn??: {0,0,0,0,0,0,0,1,0}
      }
    }
  }
  return -1; //ha -1 akkor nem kell s??vot v??ltani.
}
/*
 * Visszaadja, hogy a jobb illetve bal bit nem egyezik meg.
 * Igaz akkor, ha k??l??nbs??g a k??t bit k??z??tt nagyobb mint 1 ??s van k??z??tt??k 0
 * 
 */
 //TODO: Lehet kell rajta javitani vagy nem ezt haszn??lni mert lehet nincs 0 s??vv??lt??sn??l a k??t ??rt??k k??z??tt.
int Sensor::getDifferenceWithZero(byte sensData[]){
  int leftBit = getLeftBit(sensData);
  int rightBit = getRightBit(sensData);
  int diff = leftBit - rightBit;
  if(abs(diff) > 1){
    for(int i = rightBit; i < leftBit; i++){
      if(sensData[i] == 0){
        
        return abs(diff);
      }
    }
    
  }
  return -1;
}
/**
 * Igaz akkor, ha a k??t bit k??z??tt t??bb mint egy hely van.
 */
boolean Sensor::getDifference(byte sensData[]){
  int leftBit = getLeftBit(sensData);
  int rightBit = getRightBit(sensData);
  int diff = leftBit - rightBit;
  if(abs(diff) > 1){
        return true;

  }
  return false;
}
/**
 * Akkor Y, ha a legsz??ls?? senzor bejelez, ??s a bal valamint jobb bit k??z??tt legal??bb k??t 0 van
 */
boolean Sensor::isThisY(byte sensData[]){
  if(getDifference(sensData)){
    int leftBit = getLeftBit(sensData);
    int rightBit = getRightBit(sensData);
    if(rightBit == 0){
      int zeroNum = 0;
      for(int i=rightBit; i < leftBit; i++){
        if(sensData[i] == 0){
          zeroNum++;
        }
      }
      if(zeroNum >= 3){
        return true;
      }
    }
  }
  return false;
}
/**
 * Akkor kell meg??llnia ha az aktu??lis ??rt??kek 0-k de volt a bufferben olyan, hogy mind 1.
 * Az, hogy az utols?? h??ny ??rt??ket vizsg??lja az a STOP_LIMIT
 */
boolean Sensor::needToStop(){
  if(allSensIsZero(sens)){
    for(int i=0; i < STOP_LIMIT; i++){
      if(allSensIsOne(bufferSens[i])){
        return true;
      }
    }
  }
  return false;
}
void Sensor::createSens(int indexWhereIsTheOne){
  for(int i = 0; i < SENSOR_NUM; i++){
    if(indexWhereIsTheOne == i){
      sens[i] = 1;
    }else{
      sens[i] = 0;
    }
  }
}
boolean Sensor::needToStart(){
  int oneNum = 0;
  for(int i = 0; i< SENSOR_NUM;i++){
    if(sens[i] == 1){
      oneNum++;
    }
  }
  if(oneNum>=SENSOR_START_LIMIT){
    return true;
  }
  return false;
}
int Sensor::getLastNormalBufferIndex(int last){
  for (int i = last; i< BUFFER_NUM; i++){
    if(getDifferenceWithZero(bufferSens[i]) == -1){
      return i;
    }
  }
  return SENSOR_NUM/2;
}

void Sensor::writeDatas(byte sensData[]){

  for(int i = 0; i< SENSOR_NUM;i++){
    Serial.print(sensData[i]);
  }
  Serial.println();
  
}
boolean Sensor::allSensIsZero(byte sensData[]){
  for(int i = 0; i < SENSOR_NUM; i++){
    if(sensData[i] == 1){
      return false;
    }
  }
  return true;
}
boolean Sensor::allSensIsOne(byte sensData[]){
  for(int i = 0; i < SENSOR_NUM; i++){
    if(sensData[i] == 0){
      return false;
    }
  }
  return true;
}
void Sensor::addBuffer(byte actualSensData[]){

  memmove(bufferSens[1],bufferSens[0], SENSOR_NUM*(BUFFER_NUM-1)*sizeof(byte));
  for(int i = 0; i<SENSOR_NUM; i++){
 //   bufferSens[bufferSensCount][i] = actualSensData[i];
    bufferSens[0][i] = actualSensData[i];
  }
//  bufferSensCount++;
//  if(bufferSensCount>BUFFER_NUM){
//    bufferSensCount = 0;
//  }
}

Sensor sensor = Sensor();
