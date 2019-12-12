/*
* Project IOT_alarmclock
* Description:
* Author:
* Date:
*/

#include "LiquidCrystal_I2C_Spark.h"
#include "lux_sensor.h"
#include "webhook.h"
#include "distanceSensor.h"
#include "neopixel.h"
#include "lm35.h"


//Neopixels
SYSTEM_MODE(AUTOMATIC);

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D2
#define PIXEL_COUNT 4
#define PIXEL_TYPE WS2812B

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);


LiquidCrystal_I2C *lcd;
long time_old_pub =0;
long time_new_pub =0;
long time_old_screen =0;
long time_new_screen =0;
long setBacklightTimer =0;
long time_new_backlight_on =0;
long LED_Timer = 0;
int light_counter = 0;
long Alarm_Timer = 2000;

int sound_pin = D7;
int Dimmerlvl = 0;
int LED_ON = 0;
int OneShot = 1;

bool motionSet = false;
bool firstSound = true;
bool screen_one = true;
bool light_off = true;
bool first_hook = false;
bool alarm_gone_off = false;

String alarmCurrent;
String alarmNext;
// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  Serial.begin(9600);
  Serial.println("hej fra setup");
  lcd = new LiquidCrystal_I2C(0x27, 16, 2);
  lcd->init();
  lcd->backlight();
  lcd->clear();
  lcd->print("***Spark Time***");
  pinMode(sound_pin,OUTPUT);
  digitalWrite(sound_pin,HIGH);
  Serial.println("Starting Adafruit TSL2591 Test!");
  if (setupLuxSensor() == false)
  {
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
  }
  setupWebhook();
  setupDistance();

  //Initialize Neopixels
  strip.begin();
  strip.show();
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  time_new_pub = millis();
  time_new_screen = millis();
  time_new_backlight_on = millis();
  if (time_new_pub >= (time_old_pub + 300000))
  {
    loopWebhook();
    time_old_pub = time_new_pub;
  }
  if (time_new_screen >= (time_old_screen + 5000))
  {
    if (screen_one)
    {
      lcd->clear();
      lcd->setCursor(0,0);
      lcd->print(timeReturn());
      lcd->setCursor(0,1);
      lcd->print("Temp O:");
      lcd->print(tempReturn());
      lcd->print("C I:");
      lcd->print(IndoorTemp());
      time_old_screen = time_new_screen;
      screen_one = false;
    } else {
      lcd->clear();
      lcd->setCursor(0,0);
      lcd->print("Sunrisealarm at:");
      lcd->setCursor(5,1);
      lcd->print(alarmToDisplay());
      time_old_screen = time_new_screen;
      screen_one = true;
    }
  }
  //Check if new alarm needs to be set every day @ 12.00
  setNewAlarm();

  //Motion detection
  if (Wave()){
    if (motionSet == false){
      setBacklightTimer = millis() + 10000;
      motionSet = true;
    }
  }

  //Backlight handling
  if (time_new_backlight_on <= setBacklightTimer && motionSet == true)
  {
    lcd->setBacklight(1);
  }else{
    lcd->setBacklight(0);
    motionSet = false;
  }
  //function to increase LED level
  if(LED_ON){
    if(millis() >= LED_Timer){
      if(Dimmerlvl < 255){
        Dimmerlvl++;
        setLED(Dimmerlvl);
      }else {
        if(light_counter >= 1024){ //Adjust time to keep the LED on x*235ms
          LED_ON=0;
          light_counter = 0;
        } else{
          light_counter++;
        }
      }
      LED_Timer = millis() + 235;
    }
  } else {
    light_off = true;
    setLED(0);
    Dimmerlvl = 0;
  }

  if(OneShot == 1){
    if (first_hook == false)
    {
      loopWebhook();
      first_hook = true;
    }
    if (tempReturn())
    {
      initAlarm();
      Serial.println("Alarm set!");
      OneShot = 0;
    }
  }
}

void setLED (int dim){ //Function to set White LED level - 0 to 255 in
  for (size_t i = 0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i,dim,dim,dim);
  }
  strip.show();
}

void ActAlarmLED(){
  digitalWrite(sound_pin,LOW);
  delay(50);
  digitalWrite(sound_pin,HIGH);
  LED_ON = 1;
  LED_Timer = millis() + 235;
  light_off = false;
}

void initAlarm()
{
  alarmCurrent = sunriseReturn();
}

String alarmToDisplay(){
  if ((strncmp(timeReturn().remove(0,11),alarmCurrent,5) == 0) && (alarm_gone_off == false))
  {
    Serial.println("alarm gone off");
    alarmNext = sunriseReturn();
    if (checkLuxValue(75) == false)
    {
      ActAlarmLED();
    }
    alarm_gone_off = true;
    return alarmNext;
  }
  else
  {
    if (alarm_gone_off)
    {
      return alarmNext;
    } else {
      return alarmCurrent;
    }
  }
}

void setNewAlarm(){
  if (timeReturn() == "12:00")
  {
    alarmCurrent = alarmNext;
    alarm_gone_off = false;
  }
}
