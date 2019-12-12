#include "distanceSensor.h"

double cm = 0.0;
HC_SR04 rangefinder = HC_SR04(trigPin, echoPin, 10.0, 25.0);
void setupDistance()
{
  //rangefinder = HC_SR04(trigPin, echoPin, 10.0, 25.0);
  Spark.variable("cm", &cm, DOUBLE);
}

bool Wave(){
  bool waveOK;
  cm = rangefinder.getDistanceCM();
  //Serial.println(cm);
  if (cm > -1){
    waveOK = true;
    return waveOK;
  }
  waveOK = false;
  return waveOK;
}
