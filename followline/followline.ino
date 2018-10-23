#include "Arduino.h"
#include "Sensor.h"



void setup() {
  // put your setup code here, to run once:
  sensor.Setup();
}

void loop() {
  sensor.readSensors();
  sensor.writeDatas();
  delay(50);
}
