/*
 * Project screen_light_test_program
 * Description:
 * Author:
 * Date:
 */

#include "LiquidCrystal_I2C_Spark.h"
#include "lux_sensor.h"
#include "webhook.h"

LiquidCrystal_I2C *lcd;
long time_old_pub =0;
long time_new_pub =0;
long time_old_screen =0;
long time_new_screen =0;

bool screen_one = true;
// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  Serial.begin(9600);
  lcd = new LiquidCrystal_I2C(0x27, 16, 2);
  lcd->init();
  lcd->backlight();
  lcd->clear();
  lcd->print("***Spark Time***");

  Serial.println("Starting Adafruit TSL2591 Test!");
  if (setupLuxSensor() == false)
  {
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
  }
  setupWebhook();
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  time_new_pub = millis();
  time_new_screen = millis();
  if (time_new_pub >= (time_old_pub + 20000))
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
      lcd->print("C I:20C ");
      time_old_screen = time_new_screen;
      screen_one = false;
    } else {
      lcd->clear();
      lcd->setCursor(0,0);
      lcd->print("Sunrisealarm at:");
      lcd->setCursor(5,1);
      lcd->print(sunriseReturn());
      time_old_screen = time_new_screen;
      screen_one = true;
    }
  }
}
