#include "webhook.h"

int outdoorTemp = 0;
String sunrise;

int tempReturn(){
  return outdoorTemp;
}

String sunriseReturn(){
  return sunrise;
}

String timeReturn(){
  String currentTime = (Time.timeStr()).remove(16,24);
  return currentTime;
}

void myHandler(const char *event, const char *data){
  //  DEBUG_PRINTLN("Extracting Weatherdata from Webhook:");
  //String weatherBuffer = String(data);
  //char * currentBuffer[125];
  //currentBuffer = (char *)data;
  //weatherBuffer.toCharArray(currentBuffer, 125);
  char * weatherCondition = strtok((char *) data, "~");
  //outdoorTemp = atoi(strtok(currentBuffer, "\"~"));
  outdoorTemp = atoi(weatherCondition);
  Serial.println(outdoorTemp);
  String sunrise_all;
  sunrise_all = String(strtok(NULL,"~"));
  Serial.println(sunrise_all);
  //split up string to get sunrise time the next day
  sunrise = sunrise_all.remove(0,8);//weatherCondition.remove(0,8);
  sunrise = sunrise.remove(6,8);
  Serial.println(sunrise);
  /*  DEBUG_PRINT("Outdoor temperature: ");
  DEBUG_PRINT(outdoorTemp);
  DEBUG_PRINTLN(" C");
  DEBUG_PRINTLN();*/
}

void setupWebhook() {
  // Subscribe to the integration response event
  Particle.subscribe("weatherResponse", myHandler, MY_DEVICES);
  // Timesetup
  Time.zone(1);
  Particle.syncTime();
  waitUntil(Particle.syncTimeDone);
}

void loopWebhook() {
  // Get some data
  String data = String(50);
  // Trigger the integration
  Particle.publish("IoT5_weather", data, PRIVATE);
  // Print current time

  //  DEBUG_PRINTLN(currentTime);
  // Wait 60 seconds
  //delay(60000);
}
